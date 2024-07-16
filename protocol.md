# 协议
### 格式
#### "事件类型 内容"
### 事件终止符
#### !
## 登录界面
### 注册
#### reg Username Password
#### reg success/fail
### 登录
#### login Username Password
#### login success admin/user
#### login fail pass-err
## 普通用户
### 书籍借阅
#### borrow bookName bookType
#### borrow success/fail
### 书籍查询
#### check username passwd bookName bookType ;建议改一下，
#### check fail
#### check success bookName bookType bookStatus bookAuthor
### 书籍归还
#### username passwd return
#### return success/fail
### 书籍总览
#### overview null
#### overview book_num
#### bookName_1 bookType_1 bookStatus_1 bookAuthor_1
#### bookName_2 bookType_2 bookStatus_2 bookAuthor_2
#### ...
#### bookName_n bookType_n bookStatus_n bookAuthor_n
## 管理员
### 图书增加
#### add bookName bookWriter bookType
#### add success/fail
### 图书删除
#### del bookName
#### del success/fail
### 图书修改
#### mod modbookName bookName bookWriter bookType
#### mod success/fail
### 图书总览
#### all null
#### all book_num
#### bookName_1 bookType_1 bookStatus_1 bookAuthor_1
#### bookName_2 bookType_2 bookStatus_2 bookAuthor_2
#### ...
#### bookName_n bookType_n bookStatus_n bookAuthor_n