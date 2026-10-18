
---
AD is essentially a sizeable read-only database accessble ot all user in the domain.

A directory service, such as [Active Directory Domain Services (AD DS)](https://docs.microsoft.com/en-us/windows-server/identity/ad-ds/get-started/virtual-dc/active-directory-domain-services-overview) gives an organization ways to store directory data and make it available to both standard users and administrators on the same network.

A basic AD user account can be used to enumerate the majority of objects contained within AD, including but not limited to:

|||
|---|---|
|Domain Computers|Domain Users|
|Domain Group Information|Organizational Units (OUs)|
|Default Domain Policy|Functional Domain Levels|
|Password Policy|Group Policy Objects (GPOs)|
|Domain Trusts|Access Control Lists (ACLs)|

### Active Directory Terminology
OUs: Organizational Units, such as `Domain Controllers`, `Users`, `Computers`.
#### Object
ANY resource present within an Active Directory environment such as OUs, printers, users, domain controllers, etc.
#### Attributes
Every object in Active Directory has an associated set of [attributes](https://docs.microsoft.com/en-us/windows/win32/adschema/attributes-all) used to define characteristics of the given object. A computer object contains attributes such as the hostname and DNS name. All attributes in AD have an associated LDAP name that can be used when performing LDAP queries, such as `displayName` for `Full Name` and `given name` for `First Name`.
#### Schema
The Active Directory [schema](https://docs.microsoft.com/en-us/windows/win32/ad/schema) is essentially the blueprint of any enterprise environment. It defines what types of objects can exist in the AD database and their associated attributes. It lists definitions corresponding to AD objects and holds information about each object.
#### Container
Container objects hold other objects and have a defined place in the directory subtree hierarchy.
#### Global Unique Identifier (GUID)
A [GUID](https://docs.microsoft.com/en-us/windows/win32/adschema/a-objectguid) is a unique 128-bit value assigned when a domain user or group is created. This GUID value is unique across the enterprise, similar to a MAC address. Every single object created by Active Directory is assigned a GUID, not only user and group objects. Searching in Active Directory by GUID value is probably the most accurate and reliable way.
#### Security principals
[Security principals](https://docs.microsoft.com/en-us/windows/security/identity-protection/access-control/security-principals) are anything that the operating system can authenticate, including users, computer accounts, or even threads/processes that run in the context of a user or computer account (i.e., an application such as Tomcat running in the context of a service account within the domain). In AD, security principals are domain objects that can manage access to other resources within the domain.
#### Security Identifier (SID)
A [security identifier](https://docs.microsoft.com/en-us/windows/security/identity-protection/access-control/security-principals), or SID is used as a unique identifier for a security principal or security group. Every account, group, or process has its own unique SID, which, in an AD environment, is issued by the DC(domain controller) and stored in a secure database. A SID can only be used once. Even if the security principal is deleted, it can never be used again in that environment to identify another user or group.

When a user logs in, the system creates an access token for them which contains the user's SID, the rights they have been granted, and the SIDs for any groups that the user is a member of. This token is used to check rights whenever the user performs an action on the computer. There are also [well-known SIDs](https://ldapwiki.com/wiki/Wiki.jsp?page=Well-known%20Security%20Identifiers) that are used to identify generic users and groups. These are the same across all operating systems. An example is the `Everyone` group.



