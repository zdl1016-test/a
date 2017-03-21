#include <stdio.h>
#include "a.h"
#include "b.h"
#include "c.h"

void funb()
{
    printf("call funb\n");

    funa();
    func();
}
