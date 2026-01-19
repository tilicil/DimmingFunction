嵌入式环境不允许使用malloc分配内存，但是需要对数据进行增减的情况下，可以使用对象池
对象池可以不遍历数组或者各个标志位，无论对象多少均只进行一次操作

1.初始化用例
typedef struct
{
    uint8_t name;
    uint8_t data[15];
}_attribute_(aligned(4)) MassageType;
强制4字节对齐！！！！！

静态分配1000个消息大小的内存空间，后续操作在空间进行
static uint8_t u8Msg_Pool_Memory[sizeof(MassageType) * 1000];
static Pool_ManageType tMsg_Poll;

void app()
{
    ObjectPool_Init(&tMsg_Poll, u8Msg_Pool_Memory, sizeof(MassageType), 1000);

    while(1)
    {
        MassageType *msg1 = (MassageType*)ObjectPool_Allocate(&tMsg_Poll);
        if(msg1 == NULL)
        {
            printf("tMsg_Poll is Full!");
            return;
        }

        msg1->name = 0x06;
        memset(msg1->data, 0x55, 15);

        ObjectPool_Free(&tMsg_Poll, msg1);
    }
}

2.裁剪内容
(1)头部魔数：同一个对象不允许连续释放两次，调试阶段可以使用魔数防止出错。
(2)线程安全，中断与主循环同时申请对象时，会导致数据连续写同一个地方导致数据错乱，可以屏蔽中断或者仅在中断外操作。
(3)32bitMCU的对象需要严格按照4字节对应来构建结构体。


