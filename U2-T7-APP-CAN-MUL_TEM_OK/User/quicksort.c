#include "quicksort.h"
#if 0
void QuickSort(unsigned int *a, unsigned int low, unsigned int high)
{
	unsigned int i = low;
	unsigned int j = high;
    unsigned int key = a[low];
    if (low >= high)  //���low >= high˵�����������
    {
        return ;
    }
    while (low < high)  //��whileѭ������һ�α�ʾ�Ƚ���һ��
    {
		IWDG_ReloadCounter();
//		vTaskDelayUntil( &xLastWakeTime, 5/portTICK_RATE_MS);
        while (low < high && key <= a[high])
        {
            --high;  //��ǰѰ��
        }
        if (key > a[high])
        {
            a[low] = a[high];  //ֱ�Ӹ�ֵ, ���ý���
            ++low;
        }
        while (low < high && key >= a[low])
        {
            ++low;  //���Ѱ��
        }
        if (key < a[low])
        {
            a[high] = a[low];  //ֱ�Ӹ�ֵ, ���ý���
            --high;
        }
    }
    a[low] = key;  //������һ�ֺ�keyֵ��λ, ���ñȽ�һ�ξͻ���һ�Ρ���ʱkeyֵ�����зֳ�����������
    QuickSort(a, i, low-1);  //��ͬ���ķ�ʽ�Էֳ�������ߵĲ��ֽ���ͬ�ϵ�����
    QuickSort(a, low+1, j);  //��ͬ���ķ�ʽ�Էֳ������ұߵĲ��ֽ���ͬ�ϵ�����
}
#endif
void Bubble_2 ( unsigned int r[], unsigned  int n){
	unsigned int low = 0; 
	unsigned int high= n -1; //���ñ����ĳ�ʼֵ
	unsigned int tmp,j;
	while (low < high) {
		for (j= low; j< high; ++j) //����ð��,�ҵ������
			if (r[j]> r[j+1]) {
				tmp = r[j]; r[j]=r[j+1];r[j+1]=tmp;
			} 
		--high;					//�޸�highֵ, ǰ��һλ
		for ( j=high; j>low; --j) //����ð��,�ҵ���С��
			if (r[j]<r[j-1]) {
				tmp = r[j]; r[j]=r[j-1];r[j-1]=tmp;
			}
		++low;					//�޸�lowֵ,����һλ
	} 
} 

