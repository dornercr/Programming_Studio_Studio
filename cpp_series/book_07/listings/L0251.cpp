#include "sqlite_support.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <cerrno>

int main(){try{
    {Database setup("crash.db");setup.exec("CREATE TABLE receipts(id TEXT PRIMARY KEY,result INTEGER)");}
    pid_t child=fork();if(child<0)return 1;
    if(child==0){Database worker("crash.db");worker.exec("BEGIN IMMEDIATE;INSERT INTO receipts VALUES('job7',49);COMMIT");_exit(0);}
    int status=0;while(waitpid(child,&status,0)<0){if(errno!=EINTR)return 2;}
    if(!WIFEXITED(status)||WEXITSTATUS(status)!=0)return 3;
    Database parent("crash.db");auto result=scalar(parent,"SELECT result FROM receipts WHERE id='job7'");
    std::cout<<"reopened after child exit: result="<<result<<" rows="<<scalar(parent,"SELECT count(*) FROM receipts")<<'\n';
 }catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
