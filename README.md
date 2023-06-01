
# kas
 
C语言封装的字符串，支持集中内存管理。

`kas` 是 `char *` 类型的变种。你甚至可以在`puts`中直接使用它们，示例如下：

```c
kas str = kas_new("hello world");
puts(str);
```

## 创建字符串

创建字符串示例：

```c
kas str = kas_new("hello world");
```

## 释放字符串

```c
kas_free(&str); // str 将被赋值为 NULL
```

## 修改字符串

### 1. kas_append
 
函数原型：`kas	kas_append	(kas* _dest_string, const char* _src_string)`  
向字符串尾部追加内容  
note：这个动作可能使 `_dest_string` 指向的地址值发生改变，不过这不用关心内存泄漏的问题，如果在这之前使用了另一个指针指向这个地址，那么这块内存有可能会被释放掉

```c
kas_append(&str, "abc");
```

### 2. kas_replace

函数原型：`kas	kas_replace	(kas* _s, const char* s0, const char* s1)`  
替换字符串中的内容  
note：这个动作可能使 `_s` 指向的地址值发生改变，不过这不用关心内存泄漏的问题，如果在这之前使用了另一个指针指向这个地址，那么这块内存有可能会被释放掉

```c
kas_replace(&str, "-", ",");
```

### 3. kas_reset

函数原型：`kas kas_reset(kas* s, ...)`  
重新设置字符串中的内容。  
你可以只传递一个参数，表示将该字符串设置为空。 

```c
kas_reset(&str); // 这个字符串的首地址字符将被设置为‘\0'
```

你也可以传递两个参数，将该字符串重置为第二个参数。

```c
kas_reset(&str, "Yaho!");
```

## 从已有字符串中组合新的字符串

### 1. kas_concat

函数原型：`kas	kas_concat(const kas _s0, const kas _s1)`  
与 kas_append 不同的是，该函数不会修改任何参数，它将两个字符串拼接起来并生成一个新的字符串返回。  
参数可以从 kas 字符串或普通字符串之间任意组合

```c
kas newstr0 = kas_concat(str0, str1);
kas newstr1 = kas_concat("hello", " world");
kas newstr2 = kas_concat(str0, " ahh");
```

### 2. kas_sub

函数原型：`kas kas_sub(kas s, __uint32_t begin, __uint32_t end)`  
该函数将从一个字符串中提取字串，并返回一个新的字符串。  
提取的范围 [begin,end)，如果 end 超过了字符串的长度，那么表示提取 begin 到串末的所有字符。  

```c
kas substr = kas_sub(str0, 0, 99);
```

### 3. kas_join

函数原型：`kas_join(kas * s, char * sep, ...)`  
多个字符串组合成一个新的字符串，并以sep字符串进行拼接。  
s 类型是 `kas *` ，sep 是一个字符串，以后的参数可以是任意个数的字符串，直到遇到 NULL

```c
kas str0 = kas_join(&str0, "-", "a", "b", "c"); // "a-b-c"
```

### 4. kas_clone

函数原型：`kas	kas_clone(const kas _s)`  

## 字符串属性

### 1. kas_len

函数原型：`int	kas_len(const kas _s)`  

### 2. kas_empty

函数原型：`bool	kas_empty(const kas _s)`  

### 3. kas_startswith

函数原型：`bool kas_startswith(const kas _s, const char* _start)`  

### 4. kas_endswith

函数原型：`bool kas_endswith(const kas _s, const char* _ends)`  

### 5. kas_index

函数原型：`int	kas_index(const kas _s, const char * _sub_s)`  

### 6. kas_find

函数原型：`bool kas_find(const kas _s, const char* _sub_s)`  
 
### 7. kas_count

函数原型：`int	kas_count(const kas _s, const char* _sub_s)`  

 
## 内存管理

### 1. kas_liberate

函数原型：`kas_liberate(kas _s)`  
将一个字符串从内存管理中解放出来，调用 kas_freeany 不会被释放
 
### 2. kas_deprive

函数原型：`kas_deprive(kas _s)`  
剥夺一个字符串的自由加入内存管理中，调用 kas_freeany 将被释放掉

### 3. kas_freeany

函数原型：`void kas_freeany()`  
释放所有被管理的字符串

### 4. kas_freeall

函数原型：`void kas_freeall()`  
释放所有字符串，包括已被解放的字符串

### 5. kas_exclfree

函数原型：`void kas_exclfree(size_t num, ...)`  
效果同 kas_freeany，但是可以传递参数，将一些字符串先解放掉。

### 6. kas_isfree

函数原型：`bool kas_isfree(kas s)`  
