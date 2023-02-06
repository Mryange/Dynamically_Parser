# 动态类型语言解析器



### 放在前面

这个东西是我一年前写的，那个时候还不会一些现代C++的语法，所以写得非常的丑。



### 使用教程

结构很简单，直接编译运行即可。代码写在$test.ycc$中即可



### 输出

```python
def main(){
    println "hello";
    print 114;
    print 514;
}
```

print 不换行 ，println换行

###类型

```python
def main(){
    x = 114;
    println x;
    x = 3.14;
    println x;
    x = "hello";
    println x;
}
```



输出

```
114
3.14
hello
```



此外，有一个类似python/lua中的字典/表的table

```python
def main(){
    mp["114"] = 514;
    str = "114";
    println 114 * 1000 + mp[str];
    mp[114514] = "hello 2023";
    println mp[114 * 1000 + mp[str]];
}
```

```
114514
hello 2023
```

字符串可以用len()函数来获得长度

```python
def main(){
    s = "hello world";
    println len(s);
}
```



### 运算

```python
def main(){
    x = 10;
    x = x + 2;
    println x;
    x = x/3;
    println x;
    x = x % 2;
    println x;
}
```



输出

```
12
4
0
```

### 循环

#### while

```python
def main(){
    sum = 0;
    i = 1;
    while(i <= 10){
        sum = sum + i;
        i=i+1;
    }
    println sum;
}
```

while类似C的while

#### for

语法和python的range类似  [st,ed,step] 表示开始，结束，步长

```python
def main(){
    sum =0;
    for i in [1,10,1]{
        sum = sum + i;
    }
    println sum;
}
```

利用上面提到的表，可以写个筛法出来

```python
def main(){
    n  = 100;
    for i in [2,n,1] {
        if(notprime[i] != 1) {
            for j in [2 * i,n,i] {
                notprime[j] = 1;
            }
        }
    }
    for i in [2,100,1] {
        if(notprime[i] != 1){
            print i + " ";
        }
    }
    println ;
}
```

输出

```
2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 53 59 61 67 71 73 79 83 89 97
```

ps : 表的默认值不是0，是空表，但是在代码层面无法表示，所以需要不等于1

#### 遍历一个表

```python
def main(){
    mp[1] = 3;
    mp["hello"] = "world";
    mp[3.14] = 1919.810;
    for k,v in mp {
        println k + "," + v;
    }
}
```

输出

```python
1,3
3.140000,1919.810000
hello,world
```

同样你可以用for + len 来遍历字符串

```python
def main(){
    s = "helloworld";
    str = s[0];
    for i in [1,len(s) - 1,1] {
        str = str + "," +s[i]
    }
    println str;
}
```

输出

```
h,e,l,l,o,w,o,r,l,d
```

ps：可以尝试写个kmp出来

具体代码在https://zhuanlan.zhihu.com/p/560807061 我的知乎上有展示过

#### 函数

这里的函数定义类似python

注意的是，整个函数只能有一个return (因为多个return解析起来比较麻烦)

fib

```python
def fib (n) {
    if(n == 1 || n == 0){
        ret = 1;
    }else {
        ret = fib(n - 1) + fib(n - 2);
    }
    return ret;
}
def main(){
    for i in [1,10,1] {
        println "fib " + i + " = " + fib(i);
    }
}
```

输出

```
fib 1 = 1
fib 2 = 2
fib 3 = 3
fib 4 = 5
fib 5 = 8
fib 6 = 13
fib 7 = 21
fib 8 = 34
fib 9 = 55
fib 10 = 89
```

函数多参数

注意，多个参数，在函数定义的时候只需要用空格隔开

```python
def max(a b){
    if(a > b){ret = a;}
    else {ret = b;}
    return ret;
}
def main(){
    println max(3,5);
    println max(3.14,2.99);
}
```

输出

```
5
3.14
```

ps：为什么要这样设计呢，因为函数在定义的时候和py很像，所以我就设计的不需要逗号，然后去钓鱼了。

#### 全局变量

你可以使用global 来声明一个全局的变量。

```python
def foo(a b){
    table["foo"] = a + b;
}
def main(){
    global table;
    foo(114,514);
    println table["foo"];
}
```

你可以用这个来写一个并查集

```python
def find(x){
	if(fa[x] == x){
		ret = x;
	}else {ret = find(fa[x]);}
	return ret;
}
def merge(u v){
	fau = find(u);fav = find(v);
	if(fav!=fau){
		fa[u] = fav;
	}
}
def same(u v){
	if(find(u)==find(v)){
		println "YES";
	}else {
		println "NO";
	}
}
def main(){
	global fa;
	for i in [1,10,1] {fa[i] = i;}
	merge(1,3);merge(3,5);merge(5,7);
	same(1,7);
	same(2,7);
}
```



输出

```
YES
NO
```



当然，你可以用这个写一个BF的解析器。。。















"# parser" 
"# Dynamically_Parser" 
