### 1.调用
当C语言调用一个函数时,会将函数返回时的地址压如栈(即[esp]),函数返回时会将eax的值作为返回值.<br>
函数参数:第一个参数[esp+8],第二个[esp+8],以此类推... 
### 2.局部变量
C语言的局部变量存储在栈中,栈地址由esp寄存器存储.