//#include <stdio.h>
//#include "ex2.h"
//
//int main()
//{
//    printf("%d\n", multi(10, 5));
//    printf("%d\n", add(-1, 4));
//    printf("%d\n", sub(8, -3));
//    printf("%d\n", equal(4, 4));
//    printf("%d\n", greater(4, 4));
//    printf("%d\n", multi(1, add(3, 5)));
//}

#include <stdio.h>

///this is code in c, your code need to be in assembly
/// good luck!
//int even (int num, int i){
//    return num << i;
//}
//
//
//int go (int A[10]) {
//    int sum = 0;
//    int i = 0;
//    while (i < 10) {
//        if (A[i] % 2 == 0) {
//            int num = even (A[i], i);
//            sum += num;
//        } else {
//            sum += A[i];
//        }
//        i++;
//    }
//    return sum;
//}


int main()
{
    int array[10] = {2,1,2,1,1,1,1,1,1,1};
    int answer = go(array);
    printf("this is you answer: %d", answer);

    return 0;
}