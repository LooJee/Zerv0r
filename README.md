# Zerv0r: a simple HttpServer

It is simple http server wrote by c.It has some basic functions.
But These functions are not very efficient.I will Keeping to optimize it, and adding more functions.

todos:

- [ ] 现在路由是遍历数组的，所以效率会很低，后面可以考虑初始化时将路由表生成二叉树来提高查找效率？

- [x] 现在是接收到一个请求开启一个线程，效率略低，后面会把我自己的线程池加进来。

- [x] 优化解析 http 头的逻辑。

- [ ] 等我把 json 的轮子造好后，添加 json 应答的接口。

- [ ] 看看 fcgi 相关的东西，后面加上。