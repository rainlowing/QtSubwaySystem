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



- 















