#include "co_routine.h"
#include <unordered_map>
struct Req {
    int co_id;
    char data;
};
struct Res {
    int co_id;
    char data;
};

struct stEnv_t
{
	unordered_map<int, stCoRoutine_t*> pending_map;
};

void tbus_recv(void* buff, size_t& len) {
    
}
void tbus_send(const void* buff, const size_t len) {
    
}

void F() {
    Req req;
    Res res;
    CoSendDB(); 
}
void CoSendDB(const Req& req, Res& res) {
	stCoRoutine_t* send_db_routine;                 
	co_create(&send_db_routine, NULL, SendDBMsg, env);
	co_resume(send_db_routine);                         
}

int main(int argc,char *argv[])

	stCoRoutine_t* send_db_routine;                        //声明消费者协程结构
	co_create(&send_db_routine, NULL, SendDBMsg, env);      //创建消费者协程
	co_resume(consumer_routine);                            //启动消费者协程


    while (true) {
        static char buff[10];
        size_t len;
        tbus_recv(buff, len);
    }
    return 0;
}
