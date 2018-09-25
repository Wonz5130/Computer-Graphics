#  画一个红色的矩形框
参考画一个黑色的矩形框的配置文件，只要把 `core1.frag` 改成下面的代码即可。

```C++
#version 330 core
in vec2 texCoords;
out vec4 color;
void main() {
	color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
};
```
