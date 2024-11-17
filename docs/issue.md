# GraphicsViewZoom 类

- `GraphicsViewZoom`类的初始化中，为什么使用`view->viewport()->installEventFilter(this)`而不是直接对`view`安装事件过滤器？

在 Qt 中，`QTableView`或其他视图组件是一个容器，它包含多个子组件，如滚动条、边框、标题等。

`viewport()`是视图的数据显示区域（即视口），显示`view`容器内实际的数据内容。

如果将事件过滤器应用于整个视图会导致捕获`view`内所有组件的事件，包括滚动条的交互、标题栏的点击等。

如果只对数据显示区域的事件感兴趣，那么仅将事件过滤器安装到`viewport()`上更为高效和准确。

- `GraphicsViewZoom::eventFilter`中，为什么要在视口坐标和场景坐标之间转换？

视口展示场景的一部分，视口坐标和场景坐标描述了平面上两个不同的坐标系，或者说，视口坐标是场景坐标的局部坐标系。

由于鼠标移动事件的`pos()`函数返回当前鼠标关于视口的位置，如果记录场景坐标的变化值`delta`，每次计算`delta`都要使用坐标转换函数`mapToScene`，会带来不必要的性能消耗。

因此只在鼠标的视口坐标变化超过一定值时再转换为场景坐标。

- 场景和视口的缩放关系

鼠标滚轮向前时，`deltaAngle().y() > 0`，计算得到缩放因子`factor > 1`, 此时执行`view.scale(factor, factor); `，视口缩小，看起来是场景被放大

鼠标滚轮向后时，`deltaAngle().y() < 0`，计算得到缩放因子`factor < 1`，此时执行`view.scale(factor, factor); `，视口放大，看起来是场景被缩小



# ManageLines 类

- 为什么要将 `MainWindow` 类作为友元类？


- 为什么部分 `QAction` 对象使用 `triggered()` 槽函数不需要显式 `connect()` ？


# SubwayGraph 类

- `QHash` 的 `for` 循环遍历问题


- `QVector` 的动态内存分配问题

由于每次`push_back`时`QVector`都会重新分配动态内存，所以在`QVector`进行动态扩容的同时使用其他容器存储元素的地址，容易导致内存错误

- 最小堆优化的 Dijkstra 算法

有源点 s，求 s 到图中所有结点的最短距离，图为有向有权图。

1. 初始化：将源点 s 的距离设置为0，其他所有点的距离设置为无穷大，将源点加入堆
2. 从堆顶弹出已找到最短距离的结点 u，将结点 u 从堆中删除，加入到表示已找到最短距离的结点集合 T
3. 遍历结点 u 的所有邻居结点 v，如果 通过 u 到达 v 的距离 + s 到达 u 的最短距离 < 已知的 s 到 v 的距离，则更新 s 到 v 的距离值，并将 v 压入堆
4. 重复步骤2和步骤3，直至堆为空，或者所有结点都被加入到集合 T

在代码中，输入数据为源点`s`，图`graph`，辅助数据结构为`s`到图中所有结点的距离表`dist[]`、最小堆`pq`和集合`T`

伪代码步骤如下：

1. 初始化`dist`数组，将到达所有结点的距离都初始化为无限大，将源点的距离初始化为零
   ```c++
    for (auto& node : graph) {
        dist[node] = INF_MAX;
    }
    dist[s] = 0;
    ```
2. 将源点`s`加入堆
    ```c++
    pq.push(s);     // 或者 pq.emplace(s);
    ```
3. 如果堆中还有结点，将堆顶结点`u`弹出堆，加入到`T`
    ```c++
    while (!pq.empty) {
        auto [u, dist] = pq.top(); 
        pq.pop(u);
        T.insert(u);
        // ...
    }
    ```
4. 去除堆中关于结点`u`的过时信息
    ```c++
    if (dist > dist[u]) continue;
    ```
5. 遍历结点`u`的所有邻居结点`v`，更新距离并入堆
    ```c++
    for (const auto& [v, w] : graph) {
        if (dist[u] + w > dist[v]) {
            dist[v] = dist[u] + w;
            pq.insert(v);
        }    
    }
    ```


















