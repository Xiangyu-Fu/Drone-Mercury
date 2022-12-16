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
