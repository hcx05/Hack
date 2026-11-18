
---
ACLs
- Define who has sccess to which asset/resource 
- Define the level od access thry are provisioned.

Two types of ACLs:
- `Discretionary Access Control List`(DACL)- defines which security principals are granted or denied access to an object. DACLs are made up of ACEs that either allow or deny access. When someone attempts to access an object, the system will check the DACL for the level of access that is permitted. If a DACL does not exist for an object, all who attempt to access the object are granted full rights. If a DACL exists, but does not have any ACE entries specifying specific security settings, the system will deny access to all users, groups, or processes attempting to access it.
- `System Access Control Lists` (`SACL`) - allow administrators to log access attempts made to secured objects.
### Access Control Entries (ACEs)
There are `three` main types of ACEs that can be applied to all securable objects in AD:

|**ACE**|**Description**|
|---|---|
|`Access denied ACE`|Used within a DACL to show that a user or group is explicitly denied access to an object|
|`Access allowed ACE`|Used within a DACL to show that a user or group is explicitly granted access to an object|
|`System audit ACE`|Used within a SACL to generate audit logs when a user or group attempts to access an object. It records whether access was granted or not and what type of access occurred|
Each ACE is made up of the following `four` components:

1. The security identifier (SID) of the user/group that has access to the object (or principal name graphically)
2. A flag denoting the type of ACE (access denied, allowed, or system audit ACE)
3. A set of flags that specify whether or not child containers/objects can inherit the given ACE entry from the primary or parent object
4. An [access mask](https://docs.microsoft.com/en-us/openspecs/windows_protocols/ms-dtyp/7a53f60e-e730-4dfe-bbe9-b21b62eb790b?redirectedfrom=MSDN) which is a 32-bit value that defines the rights granted to an object
### ACEs abuse
- `ForceChangePassword` abused with `Set-DomainUserPassword`
- `Add Members` abused with `Add-DomainGroupMember`
- `GenericAll` abused with `Set-DomainUserPassword` or `Add-DomainGroupMember`
- `GenericWrite` abused with `Set-DomainObject`
- `WriteOwner` abused with `Set-DomainObjectOwner`
- `WriteDACL` abused with `Add-DomainObjectACL`
- `AllExtendedRights` abused with `Set-DomainUserPassword` or `Add-DomainGroupMember`
- `AddSelf` abused with `Add-DomainGroupMember`
