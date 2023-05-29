#include<bits/stdc++.h>
using namespace std;
#define MAXSIZE 30
typedef struct PCB {
	char process_name[20];     // 进程名称
	int  arrivetime;           //到达时间
	int  run_time;             //服务时间
	int  priority;             //优先级
	int  begin_time;           //开始时间
	int  end_time;            //结束时间
	int  copyRunning_time;     //用于时间片轮转
	PCB* next;
} Program;

typedef struct PCBQueue {
	PCB* firstProg;
	PCB* LastProg;
	int size;
} PCBQueue;

//输出每个进程的到达时间
void print(PCB pro[], int num){
	int i;

	for (i = 0; i < num; i++) {
		printf("%d ", pro[i].arrivetime);
	}
}

//输入所有进程的具体信息
void EnterProcesses(PCB pro[], int num) {
	int i;

	for (i = 0; i < num; i++) {
		PCB prog;
		printf("请输入第%d个进程的名字，到达时间，服务时间，优先级\n", i + 1);
		scanf("%s", prog.process_name);
		scanf("%d", &prog.arrivetime);
		scanf("%d", &prog.run_time);
		prog.copyRunning_time = prog.run_time;
		scanf("%d", &prog.priority);
		pro[i] = prog;
	}
}

//输出正在运行的进程的信息
void OutputRunningprocesses(PCB *pro) {
	printf("\t正在执行：进程%s\n",pro->process_name);
	printf("\t进程名字  到达时间  服务时间  优先级  开始时间  结束时间\n");
	printf("\t%s\t\t%d\t%d\t%d",pro->process_name,pro->arrivetime,pro->run_time,pro->priority);
	printf("\t%d\t%d\t\n",pro->begin_time,pro->end_time);
}

//输出就绪队列中的所有进程的信息
void OutputReadyqueue(PCBQueue *ready_queue) {
	PCB *p;

	p=ready_queue->firstProg->next;
	if(!p) {
		printf("\t无进程处于就绪状态\n");
		return ;
	}
	printf("\t就绪队列：\n");
	printf("\t进程名字  到达时间  服务时间  优先级\n");
	while(p) {
		printf("\t%s\t\t%d\t%d\t%d\n",p->process_name,p->arrivetime,p->run_time,p->priority);
		p=p->next;
	}
}

//输出进程的运行顺序
void OutputSortOfRunningprogram(PCB pro1[],int num) {
	int i;

	printf("\t进程运行顺序如下：\n");
	printf("\t进程名字  到达时间  服务时间  优先级  开始时间  结束时间\n");
	for(i=0; i<num; i++) {
		printf("\t%s\t\t%d\t%d\t%d",pro1[i].process_name,pro1[i].arrivetime,pro1[i].run_time,pro1[i].priority);
		printf("\t%d\t%d\t\n",pro1[i].begin_time,pro1[i].end_time);
	}
}

//将进程pro2的信息复制到进程pro1中
void CopyProgram(PCB *pro1,PCB *pro2) {
	memcpy(pro1->process_name,pro2->process_name,sizeof(pro2->process_name));
	pro1->arrivetime=pro2->arrivetime;
	pro1->run_time=pro2->run_time;
	pro1->priority=pro2->priority;
	pro1->begin_time=pro2->begin_time;
	pro1->end_time=pro2->end_time;
}

//初始化就绪队列
void Queueinit(PCBQueue* queue) {
	if (queue == NULL) {
		return;
	}
	queue->size = 0;
	queue->LastProg = (PCB*)malloc(sizeof(PCB));
	queue->LastProg->next=NULL;//注意加了此句
	queue->firstProg = queue->LastProg;
}

//将一个进程插入到就绪队列中
void EnterQueue(PCBQueue* queue, PCB* pro) {
	//加入进程队列
	queue->LastProg->next = (PCB*)malloc(sizeof(PCB));
	queue->LastProg = queue->LastProg->next;
	queue->LastProg->arrivetime = pro->arrivetime;
	memcpy(queue->LastProg->process_name, pro->process_name, sizeof(pro->process_name));
	queue->LastProg->priority = pro->priority;
	queue->LastProg->run_time = pro->run_time;
	queue->LastProg->copyRunning_time = pro->copyRunning_time;
	queue->LastProg->begin_time = pro->begin_time;
	queue->LastProg->next=NULL;//注意加了此句
	queue->size++;
}

//将一个进程从就绪队列中删除
PCB* poll(PCBQueue* queue) {
	PCB *temp;

	temp = queue->firstProg->next;
	if (temp == queue->LastProg) {
		queue->LastProg = queue->firstProg;
		queue->LastProg->next=NULL;//注意加了此句
		queue->size--;
		return temp;
	}
	queue->firstProg->next = queue->firstProg->next->next;
	queue->size--;

	return temp;
}

//将进程按到达时间全部排序
void sortWithEnterTime(PCB pro[], int num) {
	int i,j;
	PCB temp;

	for (i = 1; i < num; i++) {
		for (j = 0; j < num - i; j++) {
			if (pro[j].arrivetime > pro[j + 1].arrivetime) {
				temp = pro[j];
				pro[j] = pro[j + 1];
				pro[j + 1] = temp;
			}
		}
	}
}

//将一个进程按运行时间长短插入就绪队列
void EnterQueueOfRuntime(PCBQueue *ready_queue,PCB *program) { //按进程的运行时间，找到就绪队列中的相应位置并插入进去
	PCB *p,*q;
	p=ready_queue->firstProg->next;
	q=ready_queue->firstProg;

	while(p) {
		if(p->run_time>program->run_time) {
			program->next=p;
			q->next=program;
			break;
		}
		q=p;
		p=p->next;
	}
	if(!p) { //如果就绪队列为空或该进程的运行时间最长，则将其插入到队尾
		ready_queue->LastProg->next=program;
		ready_queue->LastProg=program;
		program->next=NULL;
	}
	ready_queue->size++;
}

//优先级排序
void EnterQueueOfPriority(PCBQueue *ready_queue,PCB *program) {
	PCB *p,*q;
	p=ready_queue->firstProg->next;
	q=ready_queue->firstProg;

	while(p) {
		if(p->priority<program->priority) { //优先级大的先执行
			program->next=p;
			q->next=program;
			break;
		}
		q=p;
		p=p->next;
	}
	if(!p) {
		ready_queue->LastProg->next=program;
		ready_queue->LastProg=program;
		program->next=NULL;
	}
	ready_queue->size++;
}


//最短进程优先调度算法
void SPN(PCB pro[],int num) {
	int time,done_time;
	int i,pronum;
	PCBQueue *ready_queue;
	PCB *curpro,pro1[MAXSIZE];

	printf("\n最短进程优先调度算法进程调度模拟\n");

	sortWithEnterTime(pro, num);
	ready_queue = (PCBQueue*)malloc(sizeof(PCBQueue));
	if(!ready_queue) {
		printf("分配就绪队列头结点空间失败");
		exit(1);
	}
	Queueinit(ready_queue);
	EnterQueueOfRuntime(ready_queue,&pro[0]);
	time = pro[0].arrivetime;
	pronum = 1;                     //记录当前的进程
	i=0;
	while(ready_queue->size>0) {
		curpro=poll(ready_queue);//就绪队列中的队头进程进入CPU中运行
		if(time<curpro->arrivetime) { //如果该进程与上一次运行的进程结束时间之间有间隔，则将CPU运行时间变为该进程到达时间
			time=curpro->arrivetime;
		}
		done_time=time+curpro->run_time;
		curpro->begin_time=time;
		curpro->end_time=done_time;
		//打印正在运行的进程
		OutputRunningprocesses(curpro);
		//将curpro的信息复制到pro1[i]中
		CopyProgram(&pro1[i],curpro);
		i++;
		while(pronum<num) { //将在CPU中的进程运行的时间内到达的进程放入就绪队列
			if(pro[pronum].arrivetime<=done_time) {
				EnterQueueOfRuntime(ready_queue,&pro[pronum]);
				pronum++;
			} else {
				break;
			}
		}
		//打印就绪队列
		OutputReadyqueue(ready_queue);
		time=done_time;
		if(ready_queue->size==0&&pronum<num) { //防止出现前一个进程执行完到下一个进程到达之间无进程进入
			EnterQueueOfRuntime(ready_queue,&pro[pronum]);
			pronum++;
		}
	}
	OutputSortOfRunningprogram(pro1,num);
}

//时间片轮转算法
void RR(PCB pro[], int num) {
	int time,done_time,T_time;
	int i,count,tt,timeslice,pronum;
	float QT_time,sum_T_time,sum_QT_time;
	PCB *curpro,*temp_PCB;

	printf("\n时间片轮转算法进程调度模拟\n");
	count=0;
	PCB pro2[100];
	printf("\t请输入时间片的大小:");
	scanf("%d", &timeslice);
	sortWithEnterTime(pro, num);
	PCBQueue* queue = (PCBQueue*)malloc(sizeof(PCBQueue));
	Queueinit(queue);
	pro[0].begin_time = pro[0].arrivetime;
	EnterQueue(queue, &pro[0]);
	time = 0;
	pronum = 1;
	sum_T_time = 0, sum_QT_time = 0;
	while (queue->size > 0) {
		curpro = poll(queue);                      // 从就绪队列中取出一个进程
		if (time < curpro->arrivetime) {
			time = curpro->arrivetime;                 // 计算开始运行时间
		}
		if (timeslice >= curpro->run_time) {        // 如果剩余时间小于时间片  则此任务完成
			for (tt = time; tt <= time + curpro->run_time && pronum < num; tt++) {  // 模拟进程的执行过程
				if (tt >= pro[pronum].arrivetime) {     // 统计从此任务开始到结束之间有几个进程到达
					pro[pronum].begin_time = tt;
					EnterQueue(queue, &pro[pronum]);
					pronum++;
				}
			}
			time += curpro->run_time;
			curpro->run_time = 0;
			curpro->end_time = time;
			T_time = curpro->end_time - curpro->begin_time;
			QT_time = T_time / (curpro->copyRunning_time + 0.0);
			sum_T_time += T_time;
			sum_QT_time += QT_time;

			strcpy(pro2[count].process_name, curpro->process_name) ;
			pro2[count].arrivetime=curpro->arrivetime;
			pro2[count].run_time= curpro->copyRunning_time;
			pro2[count].priority=curpro->priority;
			pro2[count].begin_time=curpro->begin_time;
			pro2[count].end_time=curpro->end_time;
			count++;

			printf("\n\t执行完成：进程%s\n",curpro->process_name);
			if(queue->size!=0) {
				printf("\t就绪队列：\n");
				printf("\t进程 到达时间  服务时间  优先级\n");
				PCB* temp_PCB=queue->firstProg->next;
				for(i=queue->size; i>0; i--) {
					printf("\t%s\t%d\t%d\t   %d\n",temp_PCB->process_name,temp_PCB->arrivetime,temp_PCB->run_time,temp_PCB->priority);
					temp_PCB=temp_PCB->next;
				}
			} else {
				printf("\t无进程处于就绪状态\n");
			}

			if (queue->size == 0 && pronum < num) {   //防止出现前一个进程执行完到下一个进程到达之间无进程进入
				pro[pronum].begin_time = pro[pronum].arrivetime;
				EnterQueue(queue, &pro[pronum]);
				pronum++;
			}
			continue;
		}
		for (tt = time; tt <= time + timeslice && pronum < num; tt++) {    //模拟进程的执行过程
			if (tt >= pro[pronum].arrivetime) { // 统计从此任务开始到结束之间有几个进程到达
				pro[pronum].begin_time = tt;
				EnterQueue(queue, &pro[pronum]);
				pronum++;
			}
		}
		time += timeslice;
		curpro->run_time -= timeslice;
		printf("\n\t正在执行：进程%s, 剩余时间：%d\n",curpro->process_name,curpro->run_time);
		if(queue->size!=0) {
			printf("\t就绪队列：\n");
			printf("\t进程 到达时间  服务时间  优先级\n");
			temp_PCB=queue->firstProg->next;
			for(i=queue->size; i>0; i--) {
				printf("\t%s\t%d\t%d\t%d\n",temp_PCB->process_name,temp_PCB->arrivetime,temp_PCB->run_time,temp_PCB->priority);
				temp_PCB=temp_PCB->next;
			}
		} else {
			printf("\t无进程处于就绪状态\n");
		}

		EnterQueue(queue, curpro);    //当前程序未完成  继续添加到队列中

		if (queue->size == 0 && pronum < num) {   //防止出现前一个进程执行完到下一个进程到达之间无进程进入
			pro[pronum].begin_time = pro[pronum].arrivetime;
			EnterQueue(queue, &pro[pronum]);
			pronum++;
		}
	}
	OutputSortOfRunningprogram(pro2,count);
}

//最短剩余时间优先
void SPT(PCB pro[], int num){
	int time,done_time,T_time;
	int i,count,tt,timeslice,pronum;
	float QT_time,sum_T_time,sum_QT_time;
	PCB *curpro,*temp_PCB;

	printf("\n最短剩余时间优先算法进程调度模拟\n");
	count=0;
	PCB pro2[100];
	timeslice=1;
	sortWithEnterTime(pro, num);
	PCBQueue* queue = (PCBQueue*)malloc(sizeof(PCBQueue));
	Queueinit(queue);
	pro[0].begin_time = pro[0].arrivetime;
	EnterQueueOfRuntime(queue, &pro[0]);
	time = 0;
	pronum = 1;
	sum_T_time = 0, sum_QT_time = 0;
	while (queue->size > 0) {
		curpro = poll(queue);                      // 从就绪队列中取出一个进程
		if (time < curpro->arrivetime) {
			time = curpro->arrivetime;                 // 计算开始运行时间
		}
		if (timeslice >= curpro->run_time) {        // 如果剩余时间小于时间片  则此任务完成
			for (tt = time; tt <= time + curpro->run_time && pronum < num; tt++) {  // 模拟进程的执行过程
				if (tt >= pro[pronum].arrivetime) {     // 统计从此任务开始到结束之间有几个进程到达
					pro[pronum].begin_time = tt;
					EnterQueueOfRuntime(queue, &pro[pronum]);
					pronum++;
				}
			}
			time += curpro->run_time;
			curpro->run_time = 0;
			curpro->end_time = time;
			T_time = curpro->end_time - curpro->begin_time;
			QT_time = T_time / (curpro->copyRunning_time + 0.0);
			sum_T_time += T_time;
			sum_QT_time += QT_time;

			strcpy(pro2[count].process_name, curpro->process_name) ;
			pro2[count].arrivetime=curpro->arrivetime;
			pro2[count].run_time= curpro->copyRunning_time;
			pro2[count].priority=curpro->priority;
			pro2[count].begin_time=curpro->begin_time;
			pro2[count].end_time=curpro->end_time;
			count++;

			printf("\n\t执行完成：进程%s\n",curpro->process_name);
			if(queue->size!=0) {
				printf("\t就绪队列：\n");
				printf("\t进程 到达时间  服务时间  优先级\n");
				PCB* temp_PCB=queue->firstProg->next;
				for(i=queue->size; i>0; i--) {
					printf("\t%s\t%d\t%d\t   %d\n",temp_PCB->process_name,temp_PCB->arrivetime,temp_PCB->run_time,temp_PCB->priority);
					temp_PCB=temp_PCB->next;
				}
			} else {
				printf("\t 无进程处于就绪状态\n");
			}

			if (queue->size == 0 && pronum < num) {   //防止出现前一个进程执行完到下一个进程到达之间无进程进入
				pro[pronum].begin_time = pro[pronum].arrivetime;
				EnterQueueOfRuntime(queue, &pro[pronum]);
				pronum++;
			}
			continue;
		}
		for (tt = time; tt <= time + timeslice && pronum < num; tt++) {    //模拟进程的执行过程
			if (tt >= pro[pronum].arrivetime) { // 统计从此任务开始到结束之间有几个进程到达
				pro[pronum].begin_time = tt;
				EnterQueueOfRuntime(queue, &pro[pronum]);
				pronum++;
			}
		}
		time += timeslice;
		curpro->run_time -= timeslice;
		printf("\n\t正在执行：进程%s, 剩余时间：%d\n",curpro->process_name,curpro->run_time);
		if(queue->size!=0) {
			printf("\t就绪队列：\n");
			printf("\t进程 到达时间  服务时间  优先级\n");
			temp_PCB=queue->firstProg->next;
			for(i=queue->size; i>0; i--) {
				printf("\t%s\t%d\t%d\t%d\n",temp_PCB->process_name,temp_PCB->arrivetime,temp_PCB->run_time,temp_PCB->priority);
				temp_PCB=temp_PCB->next;
			}
		} else {
			printf("\t 无进程处于就绪状态\n");
		}

		EnterQueueOfRuntime(queue, curpro);    //当前程序未完成  继续添加到队列中

		if (queue->size == 0 && pronum < num) {   //防止出现前一个进程执行完到下一个进程到达之间无进程进入
			pro[pronum].begin_time = pro[pronum].arrivetime;
			EnterQueueOfRuntime(queue, &pro[pronum]);
			pronum++;
		}
	}
	OutputSortOfRunningprogram(pro2,count);
}

//动态优先数
void DPSA(PCB pro[],int num){
	int time,done_time,T_time;
	int i,count,tt,timeslice,pronum;
	float QT_time,sum_T_time,sum_QT_time;
	PCB *curpro,*temp_PCB;

	printf("\n动态优先数算法进程调度模拟\n");
	count=0;
	PCB pro2[100];
	timeslice=1;
	sortWithEnterTime(pro, num);
	PCBQueue* queue = (PCBQueue*)malloc(sizeof(PCBQueue));
	Queueinit(queue);
	pro[0].begin_time = pro[0].arrivetime;
	EnterQueueOfPriority(queue, &pro[0]);
	time = 0;
	pronum = 1;
	sum_T_time = 0, sum_QT_time = 0;
	while (queue->size > 0) {
		curpro = poll(queue);                      // 从就绪队列中取出一个进程
		curpro->priority -= 1;
		if (time < curpro->arrivetime) {
			time = curpro->arrivetime;                 // 计算开始运行时间
		}
		if (timeslice >= curpro->run_time) {        // 如果剩余时间小于时间片  则此任务完成
			for (tt = time; tt <= time + curpro->run_time && pronum < num; tt++) {  // 模拟进程的执行过程
				if (tt >= pro[pronum].arrivetime) {     // 统计从此任务开始到结束之间有几个进程到达
					pro[pronum].begin_time = tt;
					EnterQueueOfPriority(queue, &pro[pronum]);
					pronum++;
				}
			}
			time += curpro->run_time;
			curpro->run_time = 0;
			curpro->end_time = time;
			T_time = curpro->end_time - curpro->begin_time;
			QT_time = T_time / (curpro->copyRunning_time + 0.0);
			sum_T_time += T_time;
			sum_QT_time += QT_time;

			strcpy(pro2[count].process_name, curpro->process_name) ;
			pro2[count].arrivetime=curpro->arrivetime;
			pro2[count].run_time= curpro->copyRunning_time;
			pro2[count].priority=curpro->priority;
			pro2[count].begin_time=curpro->begin_time;
			pro2[count].end_time=curpro->end_time;
			count++;

			printf("\n\t执行完成：进程%s\n",curpro->process_name);
			if(queue->size!=0) {
				printf("\t就绪队列：\n");
				printf("\t进程 到达时间  服务时间  优先级\n");
				PCB* temp_PCB=queue->firstProg->next;
				for(i=queue->size; i>0; i--) {
					printf("\t%s\t%d\t%d\t   %d\n",temp_PCB->process_name,temp_PCB->arrivetime,temp_PCB->run_time,temp_PCB->priority);
					temp_PCB=temp_PCB->next;
				}
			} else {
				printf("\t无进程处于就绪状态\n");
			}

			if (queue->size == 0 && pronum < num) {   //防止出现前一个进程执行完到下一个进程到达之间无进程进入
				pro[pronum].begin_time = pro[pronum].arrivetime;
				EnterQueueOfPriority(queue, &pro[pronum]);
				pronum++;
			}
			continue;
		}
		for (tt = time; tt <= time + timeslice && pronum < num; tt++) {    //模拟进程的执行过程
			if (tt >= pro[pronum].arrivetime) { // 统计从此任务开始到结束之间有几个进程到达
				pro[pronum].begin_time = tt;
				EnterQueueOfPriority(queue, &pro[pronum]);
				pronum++;
			}
		}
		time += timeslice;
		curpro->run_time -= timeslice;

		printf("\n\t正在执行：进程%s, 剩余时间：%d, 优先数：%d\n",curpro->process_name,curpro->run_time,curpro->priority);

		if(queue->size!=0) {
			printf("\t就绪队列：\n");
			printf("\t进程 到达时间  服务时间  优先级\n");
			temp_PCB=queue->firstProg->next;
			for(i=queue->size; i>0; i--) {
				printf("\t%s\t%d\t%d\t%d\n",temp_PCB->process_name,temp_PCB->arrivetime,temp_PCB->run_time,temp_PCB->priority);
				temp_PCB=temp_PCB->next;
			}
		} else {
			printf("\t 无进程处于就绪状态\n");
		}

		EnterQueueOfPriority(queue, curpro);    //当前程序未完成  继续添加到队列中

		if (queue->size == 0 && pronum < num) {   //防止出现前一个进程执行完到下一个进程到达之间无进程进入
			pro[pronum].begin_time = pro[pronum].arrivetime;
			EnterQueueOfPriority(queue, &pro[pronum]);
			pronum++;
		}
	}
	OutputSortOfRunningprogram(pro2,count);
}

int main() {
	int n, t = 1;
	int proNum,  choice;
	PCB pro[MAXSIZE],temp_pro[MAXSIZE];

	printf("\n初始化\n");
	printf("设置进程的个数：");
	scanf("%d", &proNum);
	EnterProcesses(pro, proNum);
	while (t) {

        printf("<<操作系统进程调度算法模拟程序>>\n");
        printf("1.时间片轮转调度\n");
        printf("2.优先数调度\n");
        printf("3.最短进程优先\n");
        printf("4.最短剩余时间优先\n");
        printf("5.退出\n");
        printf("请选择进程调度算法:");
		memcpy(temp_pro, pro, (sizeof(pro) / sizeof(pro[0])) * sizeof(PCB));
		scanf("%d", &n);
		while (n <= 0 || n > 5) {
			printf("指令不正确，请重新输入指令： ");
			scanf("%d", &n);
		}
		system("cls");
		switch (n) {
			case 1: {
				RR(temp_pro, proNum);
				break;
			}
			case 2: {
				DPSA(temp_pro, proNum);
				break;
			}
			case 3: {
				SPN(temp_pro, proNum);
				break;
			}
			case 4: {
				SPT(temp_pro, proNum);
				break;
			}
			case 5: {
				t = 0;
				break;
			}
		}
		getchar();
		printf("\n\t按下任意键继续...");
		getchar();
		system("cls");
	}
	system("cls");
	printf("\n\n已成功退出系统\n");

	return 0;
}


