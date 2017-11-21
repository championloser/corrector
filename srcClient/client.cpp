#include"../include/ReadConfigFile.h"
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>
#include<iostream>
using std::cout;
using std::endl;
using jjx::ReadConfigFile;

int main()
{
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in seraddr;
	memset(&seraddr,0,sizeof(seraddr));
	seraddr.sin_family=AF_INET;
	seraddr.sin_port=htons(atoi(ReadConfigFile::getInstance()->find("PORT:").c_str()));
	seraddr.sin_addr.s_addr=inet_addr(ReadConfigFile::getInstance()->find("IP:").c_str());
	int ret;
	ret=connect(sfd,(struct sockaddr*)&seraddr,sizeof(seraddr));
	if(ret!=0)
	{
		perror("connect");
		return 0;
	}

	int epfd=epoll_create(1);
	struct epoll_event event,evs[2];
	memset(&event,0,sizeof(event));
	memset(evs,0,sizeof(evs));
	event.events=EPOLLIN;
	event.data.fd=STDIN_FILENO;
	epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&event);
	event.events=EPOLLIN;
	event.data.fd=sfd;
	epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&event);
	
	char buf[256];
	int i,tmpret;
	while(1)
	{
		ret=epoll_wait(epfd,evs,2,-1);
		for(i=0;i<ret;i++)
		{
			if(evs[i].data.fd==STDIN_FILENO)
			{
				memset(buf,0,sizeof(buf));
				tmpret=read(STDIN_FILENO,buf,sizeof(buf));
				if(tmpret>0)
				{
					send(sfd,buf,strlen(buf)-1,0);
				}
				else{printf("Goodbye\n");break;}
			}
			if(evs[i].data.fd==sfd)
			{
				memset(buf,0,sizeof(buf));
				tmpret=recv(sfd,buf,sizeof(buf),0);
				if(tmpret>0)
				{
					cout<<buf<<endl;
					cout<<"please input a word: "<<std::flush;//std::flush刷新输出缓冲区
				}
				else{printf("Goodbye\n");break;}
			}
		}
		if(0==tmpret)break;
	}
	close(sfd);
	return 0;
}
