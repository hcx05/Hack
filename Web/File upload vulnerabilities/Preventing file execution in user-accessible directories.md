
---
This kind of configuration often differs between directories. A directory to which user-supplied files are uploaded will likely have much stricter controls than other locations on the filesystem that are assumed to be out of reach for end users. If you can find a way to upload a script to a different directory that's not supposed to contain user-supplied files, the server may execute your script after all.

You should also note that even though you may send all of your requests to the same domain name, this often points to a reverse proxy server of some kind, such as a load balancer. Your requests will often be handled by additional servers behind the scenes, which may also be configured differently.

If we make the same request but server response differently :

Check for Session Persistence: Verify if there are specific Cookies or Tokens that bind your requests to a single backend server.

Retry Multiple Times: A single failure doesn't necessarily mean the vulnerability is absent; it could be that a particular backend server has "smarter" configurations.

Account for Synchronization Delay: Files may take a few seconds to sync across all backend nodes after an upload, so waiting a moment before accessing the file is a sound strategy.

