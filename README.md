# demo_test_fcgi_cjson
This is a demo for testing fcgi and cjson project.

# Destination
处理客户端发起fcgi请求时，然后去解析参数并返回结果。可构造成cjson并返回json字符串。
也可以使用rudecgi(c++).

# Dependent installation 相关依赖安装
（略）      

# You know
fcgi处理步骤<br>
通过Internet 把用户请求送到Web 服务器<br>
Web 服务器接收到用户请求并交给CGI 程序<br>
CGI 程序把处理结果传送给Web 服务器<br>
Web 服务器把结果送回到用户<br>
# Demo
eg.<br>
得到参数 char *query_string = "CMD=LOGON&SELECT=0&USERNAME=user&PASSWORD=123";<br>
对其解析成字符串：
```json
0---CMD=LOGON
1---SELECT=0
2---USERNAME=user
3---PASSWORD=123
```
   
对其解析成json字符串
```json
{
        "CMD":  "LOGON",
        "SELECT":       "0",
        "USERNAME":     "user",
        "PASSWORD":     "123"
}
```

