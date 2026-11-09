# Kerberos 認證流程:從抽象設計到實作細節

---

## Part 0:問題定義與威脅模型(最頂層抽象)

在看任何步驟之前,先定義問題與敵人假設——這是推導出所有後續設計的根源。

**要解決的問題**:組織裡有大量使用者與大量服務,如何讓使用者「證明一次身分」就能存取多個服務,而不需要:
- 密碼在網路上傳輸
- 每個服務各自保存使用者密碼的副本或雜湊
- 使用者對每個服務重複做完整的身分驗證

**假設的威脅模型**(推導出所有設計的根源):

| 假設 | 對應的設計後果 |
|---|---|
| 網路可被竊聽(passive attacker) | 密碼絕不能明文或可逆地出現在網路上 |
| 封包可被重放或竄改 | 需要時間戳 / nonce / authenticator |
| 票據(ticket)本身可能被複製或竊取 | 票據要綁定一把「只有合法持有者才有」的 session key |
| 服務不一定可信到能保管使用者密碼 | 服務永遠看不到使用者密碼,只拿到 KDC 背書的憑證 |
| 但 KDC(DC)是被信任的中心 | 整個系統是 trusted third party 模型,信任收斂到 KDC |

**結論**:Kerberos 不是「身分驗證協定」,而是「用有時效性的票據取代重複證明身分」的機制。後面所有步驟都是在滿足上表的限制條件。

---

## Part 1:架構層的五條設計原則

這一層回答「為什麼協定長這個樣子」。

### 原則 1 — 拆成兩張票(TGT / Service Ticket):authentication 與 authorization 分離
密碼應該被使用最少次數、最短時間。若每次存取新服務都要重算密碼衍生金鑰,密碼相關金鑰會在系統各處反覆暴露。拆成兩張票後,K_user **只在 AS 階段用一次**;之後整個 session 都靠 TGT(代表「已證明身分」)去換 Service Ticket(代表「對特定服務的授權憑證」)。

### 原則 2 — 票據是「自我包含且不透明」的(self-contained, opaque token)
若 KDC 維護 session table 記住誰登入過,規模一大就是效能與單點瓶頸。Kerberos 選擇相反設計:**狀態不存在 KDC,而是加密封裝進票據,由 Client 帶著走**。KDC 只要能用自己的金鑰解開票據,就等於驗證了內容沒被竄改——用加密取代儲存的 stateless 設計,概念上類似 JWT / signed capability token。

### 原則 3 — 每次交換拆成「給下一個驗證者的部分」與「給 Client 的部分」,用不同金鑰加密
兩個讀者不能讀到對方該讀的東西:票據本身是給下一個驗證者看的,Client 必須讀不懂,否則可偽造或竄改裡面的身分與授權資料;session key 才是給 Client 用來證明自己持有這張票。這是 need-to-know 原則的加密落地。

### 原則 4 — 票據(Ticket)與憑證(Authenticator)分離
票據是可被複製的靜態位元組,若單靠它就能通過驗證,竊取 ticket 等於永久竊取身分(直到過期)。因此設計上區分:
- **Ticket**(長效、可重用):類似 bearer credential。
- **Authenticator**(短效、一次性,每次使用都重新產生):證明「我現在真的持有對應的 session key」,類似 proof-of-possession。

### 原則 5 — 對稱式加密,而非一開始就用非對稱
Kerberos 設計於 1980 年代,非對稱加密運算成本高,而且信任模型本來就假設「KDC 已跟每個 principal 共享一把長期金鑰」,對稱加密已足夠,不需額外建 PKI。(後來的 PKINIT 擴充才引入公鑰做初始驗證,是加在原架構之上,不是取代。)

---

## Part 2:金鑰傳遞鏈總覽

```
K_user ──(AS-REP)──> K_TGT-session ──(TGS-REP)──> K_service-session ──(AP-REQ)──> 服務
使用者長期金鑰        TGT 的 session key            服務票據的 session key
```

每一步都是「用上一步拿到的金鑰,換下一把金鑰」,目的是**限制長期金鑰的暴露範圍**:K_user 只在最敏感的 AS 階段用一次,之後全部改用當次產生的短期 session key。

**角色**:
- **Client**:使用者(或其機器)
- **KDC**:跑在 DC 上,含 **AS**(Authentication Service)與 **TGS**(Ticket-Granting Service)
- **Service**:目標服務(CIFS、HTTP、MSSQL 等,以 SPN 識別)

**流程圖**:
```
Client                         KDC (AS + TGS)                    Service
  |--- AS-REQ (pre-auth) -------->|                                  |
  |<-- AS-REP (TGT, sess key) ----|                                  |
  |--- TGS-REQ (TGT, SPN, auth) ->|                                  |
  |<-- TGS-REP (ST, sess key) ----|                                  |
  |--------------- AP-REQ (ST, authenticator) --------------------->|
  |<-------------- AP-REP (optional mutual auth) -------------------|
```

---

## Part 3:逐步細節,並對照「為什麼這樣設計」

### 步驟 0:前置條件

**使用者長期金鑰(K_user)**
- RC4-HMAC:`K_user = NT hash = MD4(UTF-16LE(密碼))`,無 salt。
- AES128/256:`K_user` 由 `PBKDF2-HMAC-SHA1(密碼, salt, 4096 次)` 再經 key derivation 得出;salt 為「大寫 REALM + 帳號名」。
- AD 中存有這些金鑰,Client 端由輸入的密碼即時算出。

**時間同步**:Windows 預設容許誤差 **5 分鐘**,超過回 `KRB_AP_ERR_SKEW`(37)。

**SPN**:格式為 `服務類別/主機:埠`(如 `cifs/fileserver.corp.local`),記錄在服務帳號的 `servicePrincipalName`,KDC 靠它判斷「這張票要用哪個帳號的金鑰加密」。

---

### 步驟 1:AS-REQ(Client → KDC)— 取得 TGT 的第一步

**設計問題**:如果不驗證就直接發 AS-REP,任何人都能對任意帳號要一份「用其密碼衍生金鑰加密」的資料,離線爆破——等於白送一個 oracle。Pre-auth 強迫攻擊者**先證明猜對密碼**才拿得到任何東西。

**封包內容**:

| 欄位 | 說明 |
|---|---|
| `cname` / `realm` | 使用者名稱與網域 |
| `sname` | 固定為 `krbtgt/REALM` |
| `etype` 清單 | Client 支援的加密類型,依偏好排序 |
| `nonce` | 隨機數,對應回應,防止回應被替換 |
| `till` / `kdc-options` | 要求的票據到期時間與選項旗標 |
| `padata` | pre-authentication 資料 |

**Pre-authentication(`PA-ENC-TIMESTAMP`)**:內容為「當前時間 + 微秒」,用 K_user 加密(key usage 1)。許多客戶端(含 Windows)會先不帶時間戳送一次,KDC 回 `KDC_ERR_PREAUTH_REQUIRED`(25)並附 `ETYPE-INFO2`(告知 salt 與加密類型),Client 再帶時間戳重送;也可一開始就直接帶。

**KDC 檢查順序**:
1. 帳號是否存在(否則 `C_PRINCIPAL_UNKNOWN`)
2. 帳號是否停用、鎖定、過期、超出允許登入時段
3. 用 AD 中的 K_user 解密時間戳,解不開代表密碼錯(`PREAUTH_FAILED`,24)
4. 時間戳是否在容許誤差內
5. 雙方是否有共同支援的 etype

**威脅對應**:沒有 pre-auth,就是 **AS-REP Roasting**(針對設有 `DONT_REQ_PREAUTH` 的帳號)成立的根本原因。

**日誌**:成功 4768,失敗 4771(或 4768 帶失敗碼)。

---

### 步驟 2:AS-REP(KDC → Client)— 發 TGT,交付 session key

**設計問題**:如果 TGT 和 session key 混在一起用同一把 K_user 加密,TGT 的內容(PAC、授權資料)就暴露在「可被離線爆破」的加密層下——而這些資料其實該由 KDC 跟目標服務互相信任,跟使用者密碼無關。所以必須拆成兩段獨立加密。

**2.1 TGT(給 Client 保管,但讀不了)**
- 加密金鑰:**K_krbtgt**(key usage 2)
- 內容:

| 欄位 | 說明 |
|---|---|
| `flags` | forwardable、renewable、initial、pre-authent 等 |
| **TGT session key** | KDC 新產生的隨機金鑰 |
| `cname` / `crealm` | 使用者身分 |
| `authtime / starttime / endtime / renew-till` | 時效(Windows 預設 TGT 10 小時、可續期 7 天) |
| **PAC** | 授權資料 |

**2.2 給 Client 的部分(`EncASRepPart`)**
- 加密金鑰:**K_user**(key usage 3)
- 內容:同一把 TGT session key、`nonce`、時效、`sname`
- Client 解開後核對 `nonce`,把「TGT + session key」存入票據快取(Windows 在 LSASS,Linux 在 ccache 或 keyring)

**2.3 PAC(Privilege Attribute Certificate)**
微軟對 Kerberos 的擴充,放在票據的 `authorization-data`:
- `KERB_VALIDATION_INFO`:使用者 SID、主要群組、所屬群組 RID、登入資訊等
- `Client Info`:使用者名稱與 authtime
- 兩個簽章:**Server Signature**(用票據接收方的金鑰)與 **KDC Signature**(用 K_krbtgt);在 TGT 裡兩者都用 K_krbtgt
- 較新的更新加入 `PAC_REQUESTOR`、extended KDC signature 等,細節依補丁版本而異

> **為什麼 PAC 存在,且不屬於 Kerberos 原始規格**:Kerberos 標準只處理 authentication(你是誰),不處理 authorization(你能幹嘛)。Windows/AD 需要在票據裡夾帶群組 SID,讓服務不用查 AD 就能做存取控制。微軟的解法是在 Kerberos 原本預留的 `authorization-data` 擴充欄位塞入 PAC——**在不修改核心協定前提下做 vendor extension**。代價是 PAC 的 KDC Signature 驗證,服務端做不到,必須回頭問 DC,產生額外信任鏈與效能成本。

**此步驟完成後 Client 持有**:TGT(不透明)、TGT session key(明文可用)。**此後不再需要 K_user 參與**。

---

### 步驟 3:TGS-REQ(Client → KDC)— 用 TGT 換服務票

**設計問題**:如果只憑 TGT 本身就能換 service ticket,竊取一份 TGT 檔案(即使沒有 session key)就能無限次兌換任何服務的票。Authenticator 強迫「必須同時持有 session key」才能用 TGT——這正是「票據與憑證分離」原則的落地。

**封包內容**:

| 欄位 | 說明 |
|---|---|
| `PA-TGS-REQ`(內含 AP-REQ) | 見下 |
| `sname` | 目標 SPN |
| `etype` 清單、`nonce`、`kdc-options` | 同步驟 1 |

**`PA-TGS-REQ` 內的兩樣東西**:
1. **TGT**(原封不動)
2. **Authenticator**:用 TGT session key 加密(key usage 7),內容含 `cname`、`crealm`、`ctime/cusec`,可選 `subkey` 與 `seq-number`

**KDC 處理順序**:
1. 用 K_krbtgt 解開 TGT,得到 session key、使用者身分與 PAC(解不開代表票據不是自己發的)
2. 用 TGT session key 解密 authenticator
3. 比對 authenticator 的 `cname/crealm` 與 TGT 內是否一致
4. 檢查 `ctime` 在誤差內、TGT 未過期、不在重放快取中
5. 以 SPN 找出對應帳號(找不到回 `S_PRINCIPAL_UNKNOWN`,7;若屬其他網域,回發 referral TGT)
6. 決定 service ticket 的加密類型,取 Client 請求的 etype 與服務帳號支援類型(`msDS-SupportedEncryptionTypes`)的交集——這是 RC4 降級與 Kerberoasting 相關的設定點

**此步驟的關鍵設計取捨**:**KDC 不檢查「使用者是否有權限用該服務」**,只要 TGT 有效且 SPN 存在就發票。

> **為什麼要這樣設計**:如果 TGS 要判斷授權,就要知道每個服務自己的存取控制邏輯,等於把 authorization 邏輯集中到 KDC——違反「服務自己管自己授權」的分散式設計。TGS 的職責被刻意限制在「路由 + 打包票據」,真正的 gatekeeping 留給服務自己依 PAC 判斷。這是「authentication 與 authorization 分離,且 authorization 下放給各服務」的直接體現——**也是 Kerberoasting 能成立的根本原因之一**。

**日誌**:4769。

---

### 步驟 4:TGS-REP(KDC → Client)— 交付服務票據

**設計問題**:同步驟 2,服務票據也必須拆成兩段獨立加密,理由相同(不同讀者、不同金鑰)。

**4.1 Service Ticket(Client 讀不了)**
- 加密金鑰:**服務帳號的金鑰 K_service**(key usage 2)。RC4 時是該帳號的 NT hash,AES 時是 AES key。
- 內容:新產生的 service session key、`cname/crealm`、時效(不超過 TGT 剩餘時間)、PAC
- PAC 從 TGT 複製後**重新簽章**:Server Signature 改用 K_service,KDC Signature 仍用 K_krbtgt

**4.2 給 Client 的部分**
- 加密金鑰:TGT session key(key usage 8),若 authenticator 帶了 subkey 則用 subkey(key usage 9)
- 內容:service session key、`nonce`、時效

**結果**:Client 持有 service ticket(不透明)與 service session key。

> **Kerberoasting 的原理對應**:Service ticket 的一部分**必定**用服務帳號金鑰加密,而任何已驗證的網域使用者都能請求任何有 SPN 帳號的票。因此這段密文本質上是「用服務帳號密碼衍生金鑰加密的已知明文結構」,攻擊者可以把它帶走離線慢慢猜。使用者帳號當服務帳號且密碼弱時風險最高。

---

### 步驟 5:AP-REQ(Client → Service)— 向服務出示票據

**設計問題**:如果沿用 TGS 階段那個 authenticator,等於同一份一次性憑證被用了兩次,失去防重放的意義。每個驗證關卡都要自己當下產生新的 proof-of-possession。

**傳輸方式**:AP-REQ 不是獨立網路協定,而是**嵌在應用層協定裡**,通常經 GSS-API / SPNEGO 承載:
- SMB:Session Setup 的 security blob
- HTTP:`Authorization: Negotiate <base64>`
- LDAP:SASL GSSAPI bind

**封包內容**:

| 欄位 | 說明 |
|---|---|
| `ap-options` | 含 `mutual-required`(是否要求服務也證明身分) |
| `ticket` | 步驟 4 拿到的 service ticket |
| `authenticator` | 用 service session key 加密(key usage 11),含新的 `ctime/cusec`、可選 subkey 與 seq-number |

**服務端驗證順序**:
1. 用自己的金鑰(Windows 由 LSA 取用帳號金鑰,Linux 通常來自 keytab)解開 ticket,得到 service session key、使用者身分與 PAC。解不開代表票據不是用此服務的金鑰加密,常見原因是 SPN 對到錯帳號,回 `KRB_AP_ERR_MODIFIED`(41)
2. 檢查票據時效(`KRB_AP_ERR_TKT_EXPIRED`,32)
3. 用 service session key 解密 authenticator,核對 `cname/crealm`
4. 檢查 `ctime` 在誤差內,並檢查重放快取(`KRB_AP_ERR_REPEAT`,34)
5. 驗證 PAC 的 Server Signature(用自己的金鑰)
6. KDC Signature 需要 K_krbtgt,服務本身沒有,所以只能請 DC 代驗(經 Netlogon secure channel 的 PAC validation)。以 system 身分執行的服務通常不送驗,非 system 身分的服務才會送驗,實際行為受設定與補丁影響
7. 由 PAC 的使用者 SID 與群組 SID 建立 access token,依 ACL 做授權判斷

**此階段服務通常不需要聯絡 KDC**(除了上述 PAC 驗證)——這是 Silver Ticket 難以偵測的原因:偽造服務票據的驗證完全發生在服務端本地,不會在 DC 留下 4769 紀錄。

**日誌**:服務主機上的 4624(Logon Type 3,Network),認證套件為 Kerberos。

---

### 步驟 6:AP-REP(Service → Client,可選)— 雙向認證

**設計問題**:若不驗證服務端身分,Client 可能把票據與 session key 的使用結果交給冒牌伺服器。AP-REP 讓 Client 反向確認對方真的持有 K_service。

- 內容:`EncAPRepPart`,包含 Client authenticator 中的 `ctime/cusec`,用 service session key 加密(key usage 12)
- Client 解密後比對時間是否與自己送出的完全一致。能正確回傳,代表對方解得開票據、拿得到 session key
- 之後的應用層訊息可用 session key 或協商出的 subkey 做簽章或加密(如 SMB signing/encryption)

> **為什麼 mutual auth 是選項而非必須**:Client 走到這一步前已透過 AS/TGS 信任了 KDC 這個中心,服務的金鑰某種意義上是 KDC 間接背書的。協定原始威脅模型裡,「偽造的服務」風險被認為比「偽造的 KDC」低,所以雙向驗證被設計成可選加強。這反映威脅模型的優先順序:假設的風險等級不同,防禦強度就不同。

---

### 步驟 7:後續行為(快取、續期)

- **TGT 快取**:有效期內,存取其他服務只需重複步驟 3~5,不必再輸入密碼
- **Service ticket 快取**:同一服務再次存取直接沿用,不再向 KDC 請求
- **續期**:可續期的 TGT 在 `renew-till` 前可向 KDC 換新(日誌 4770)
- **登出**:票據快取清除。已簽出的票據在到期前理論上仍可被持有者使用——這是 Pass-the-Ticket 能成立的原因

---

## Part 4:總表與速查

### 4.1 每一步「誰加密、誰能解」

| 步驟 | 內容 | 加密金鑰 | 誰能解開 |
|---|---|---|---|
| 1 AS-REQ | pre-auth 時間戳 | K_user | KDC |
| 2 AS-REP | TGT | K_krbtgt | 只有 KDC |
| 2 AS-REP | TGT session key(給 Client) | K_user | Client |
| 3 TGS-REQ | Authenticator | TGT session key | KDC、Client |
| 4 TGS-REP | Service ticket | K_service | 只有服務 |
| 4 TGS-REP | service session key(給 Client) | TGT session key | Client |
| 5 AP-REQ | Authenticator | service session key | 服務、Client |
| 6 AP-REP | 時間戳回應 | service session key | Client |

### 4.2 各金鑰的用途與攻擊面對應

| 金鑰 | 加密/簽署對象 | 取得後的後果 |
|---|---|---|
| 使用者金鑰 | pre-auth 時間戳、AS-REP 中的 session key | Pass-the-Key / Overpass-the-Hash |
| krbtgt 金鑰 | TGT | 偽造任意 TGT(Golden Ticket) |
| 服務帳號金鑰 | service ticket | 偽造該服務的票據(Silver Ticket) |

### 4.3 常見錯誤碼速查

| 碼 | 名稱 | 常見原因 |
|---|---|---|
| 6 | `C_PRINCIPAL_UNKNOWN` | 使用者不存在 |
| 7 | `S_PRINCIPAL_UNKNOWN` | SPN 不存在或重複 |
| 18 | `CLIENT_REVOKED` | 帳號停用、鎖定或過期 |
| 24 | `PREAUTH_FAILED` | 密碼錯誤 |
| 25 | `PREAUTH_REQUIRED` | 需要先帶 pre-auth(正常握手的一部分) |
| 37 | `AP_ERR_SKEW` | 時間差過大 |
| 41 | `AP_ERR_MODIFIED` | 票據解不開,常見為 SPN 對應錯帳號或金鑰不一致 |

### 4.4 其他技術層的設計取捨

**金鑰階層為什麼要一層換一層**:限制長期金鑰的暴露範圍。K_user 只在最敏感的 AS 階段用一次,之後全部改用當次產生的短期 session key。即使某次 session key 外洩,影響範圍也只限於那次連線,不牽連密碼本身或其他服務的 session——把「永久金鑰的風險」轉換成「多個互相獨立、各自時效有限的風險區塊」。

**加密類型要每次協商(etype negotiation)的副作用**:協定要相容舊系統與新系統並存,每次交握都要協商雙方都支援的加密類型。相容性與安全性在這裡直接對立:只要環境裡還有東西宣告支援 RC4,協議就可能被降級到較弱的加密——這不是某一步「設計錯誤」,而是「向後相容」這個目標本身必然帶來的代價。

---

## Part 5:回扣——Kerberoasting 是「設計選擇的副作用」

把前面所有層次串起來,解釋 Kerberoasting 為什麼成立,而不是把它當成孤立的 bug:

1. **架構原則 1 + 步驟 3 的設計**(TGS 不做 authorization 判斷,只要 TGT 有效就發票)→ 導致任何已驗證使用者都能對**任意**有 SPN 的帳號請求 service ticket。
2. **步驟 4 的設計**(service ticket 的一部分必定用服務帳號金鑰加密)→ 這段密文本質上是「用服務帳號密碼衍生金鑰加密的已知明文結構」,可離線帶走慢慢試。
3. **etype 協商的相容性代價**(為了相容性允許 RC4)→ 若服務帳號的 `msDS-SupportedEncryptionTypes` 仍允許 RC4,密文可離線爆破的難度進一步降低(RC4 的 K_service 直接是 NT hash,沒有像 AES 一樣的 PBKDF2 迭代成本)。
4. **部署層的放大因素**(非協定設計問題):許多組織把一般使用者帳號(而非專用的 gMSA/服務帳號)設成 SPN 的擁有者,而使用者帳號的密碼強度與輪替頻率通常不受服務帳號管理規範約束。

**結論**:Kerberoasting 利用的不是「協定壞掉」,而是三個各自合理的設計決策疊加後的必然副作用。這與「實作缺陷」是兩種不同的根因——前者需要靠縮小 attack surface(強密碼、AES-only、gMSA)來緩解,後者才需要補丁。

---

## 延伸方向(未在本文件展開)

- PAC 簽章驗證細節與近年補丁(含 `PAC_REQUESTOR`)
- 委派(unconstrained / constrained / RBCD)在 TGS-REQ 階段用到的 S4U2Self 與 S4U2Proxy 流程
- Golden Ticket / Silver Ticket / 約束委派攻擊,各自破壞了信任鏈上的哪一層假設
