# 
所有程序编译通过,LED3闪烁.
![图 2](../../../images/4269b104e7b83697cfc37feda97cf8d6167488f994296d18697c85bc3ca9c4cc.png)  

![图 3](../../../images/7b60717e131da4be5a42895dda7ba4beec6af30a2b76006480d142ebc8d3270f.gif)  


手柄测试完成，

```c
    dataToSend[5] = joy_left_x_value & 0xFF;
    dataToSend[6] = (joy_left_x_value >> 8) & 0xFF;
    dataToSend[7] = joy_left_y_value & 0xFF;
    dataToSend[8] = (joy_left_y_value >> 8) & 0xFF;
```

```bash
Buffer 0:14 
Buffer 1:1 
Buffer 2:1 
Buffer 3:3 
Buffer 4:26 
Buffer 5:40 
Buffer 6:15 
Buffer 7:14 
Buffer 8:0 
Buffer 9:0 
Buffer a:26 
Buffer b:194 
Buffer c:195 
Buffer d:196 
Buffer e:197 
Buffer f:198 
Buffer 10:26 
Buffer 11:32 
Buffer 12:0 
Buffer 13:0 
Buffer 14:0 
Buffer 15:0 
Buffer 16:0 
Buffer 17:17 
Buffer 18:0 
Buffer 19:0 
Buffer 1a:0 
Buffer 1b:0 
Buffer 1c:0 

```

```bash
Buffer 0:15
Buffer 1:1
Buffer 2:1
Buffer 3:3
Buffer 4:95
Buffer 5:40 
Buffer 6:15 
Buffer 7:14 
Buffer 8:0
Buffer 9:0
Buffer a:82
Buffer b:26
Buffer c:195
Buffer d:196
Buffer e:197
Buffer f:198
Buffer 10:66
Buffer 11:32 
Buffer 12:32
Buffer 13:32
Buffer 14:32
Buffer 15:32
Buffer 16:32
Buffer 17:17
Buffer 18:0
Buffer 19:0
Buffer 1a:0
Buffer 1b:0
Buffer 1c:0
```
