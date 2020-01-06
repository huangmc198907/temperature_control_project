#include "quicksort.h"
#if 0
void QuickSort(unsigned int *a, unsigned int low, unsigned int high)
{
	unsigned int i = low;
	unsigned int j = high;
    unsigned int key = a[low];
    if (low >= high)  //如果low >= high说明排序结束了
    {
        return ;
    }
    while (low < high)  //该while循环结束一次表示比较了一轮
    {
		IWDG_ReloadCounter();
//		vTaskDelayUntil( &xLastWakeTime, 5/portTICK_RATE_MS);
        while (low < high && key <= a[high])
        {
            --high;  //向前寻找
        }
        if (key > a[high])
        {
            a[low] = a[high];  //直接赋值, 不用交换
            ++low;
        }
        while (low < high && key >= a[low])
        {
            ++low;  //向后寻找
        }
        if (key < a[low])
        {
            a[high] = a[low];  //直接赋值, 不用交换
            --high;
        }
    }
    a[low] = key;  //查找完一轮后key值归位, 不用比较一次就互换一次。此时key值将序列分成左右两部分
    QuickSort(a, i, low-1);  //用同样的方式对分出来的左边的部分进行同上的做法
    QuickSort(a, low+1, j);  //用同样的方式对分出来的右边的部分进行同上的做法
}
#endif
void Bubble_2 ( unsigned int r[], unsigned  int n){
	unsigned int low = 0; 
	unsigned int high= n -1; //设置变量的初始值
	unsigned int tmp,j;
	while (low < high) {
		for (j= low; j< high; ++j) //正向冒泡,找到最大者
			if (r[j]> r[j+1]) {
				tmp = r[j]; r[j]=r[j+1];r[j+1]=tmp;
			} 
		--high;					//修改high值, 前移一位
		for ( j=high; j>low; --j) //反向冒泡,找到最小者
			if (r[j]<r[j-1]) {
				tmp = r[j]; r[j]=r[j-1];r[j-1]=tmp;
			}
		++low;					//修改low值,后移一位
	} 
} 

