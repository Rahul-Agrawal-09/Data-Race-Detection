#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "./vector_clock.cc"
using namespace std;

int tester;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
VectorClock vc_main;
pthread_t parent_id;
VectorClock tester_write;

void *thread1(void *tmp)
{
	pthread_t my_id=pthread_self();
	VectorClock vc=vc_main;
	vc.SetClock(my_id,vc_main.GetClock(parent_id));
	// cout << pthread_self() << endl;
	bool ans=tester_write.HappensBefore(&vc);
	cout << "This in T1: " << ans << endl;
	if(ans){
		tester_write=vc;
		tester += 3;
	}
	else{
		cout<<"Race in T1"<<endl;
	}
	vc.Increment(my_id);
	for(int i=0;i<1000000;i++) 
		vc.Increment(my_id);

	*(int *)tmp += 1;
	vc.Increment(my_id);
	cout<<"Tester in T1: "<<tester<<endl;
	vc.Increment(my_id);
	vc_main.Join(&vc);
	return NULL;
}
void *thread2(void *tmp)
{
	pthread_t my_id=pthread_self();
	VectorClock vc=vc_main;
	vc.SetClock(my_id,vc_main.GetClock(parent_id));
	// cout << pthread_self() << endl;
	*(int *)tmp += 2;
	vc.Increment(my_id);
	for(int i=0;i<1000000;i++) 
		vc.Increment(my_id);

	bool ans=tester_write.HappensBefore(&vc);
	cout << "This in T2: " << ans << endl;
	if(ans){
		tester_write=vc;
		tester += 3;
	}
	else{
		cout << "Race in T2"<<endl;
	}
	vc.Increment(my_id);
	vc_main.Join(&vc);
	return NULL;
}

void threadCretator()
{
	pthread_t my_id=pthread_self();
	parent_id=my_id;
	vc_main.Increment(my_id);
	cout << my_id << endl;
	tester_write=vc_main;
	tester = 0;
	vc_main.Increment(my_id);
	int *tmp=new int(4);
	vc_main.Increment(my_id);
	pthread_t id1,id2;
	vc_main.Increment(my_id);
	pthread_create(&id1,NULL,thread1,tmp);
	vc_main.Increment(my_id);
	bool ans=tester_write.HappensBefore(&vc_main);
	cout << "This in main: " << ans << endl;
	tester += 2;
	vc_main.Increment(my_id);
	pthread_create(&id2,NULL,thread2,tmp);
	vc_main.Increment(my_id);
	for(int i=0;i<1000000;i++) 
		vc_main.Increment(my_id);
	cout<<"Tester in Main "<<*tmp<<endl;
	vc_main.Increment(my_id);
	// cout << vc_main.ToString() << endl;
	pthread_join(id1,NULL);
	pthread_join(id2,NULL);
	vc_main.Increment(my_id);
	delete tmp;
	cout << vc_main.ToString() << endl;
}

int main()
{
	threadCretator();
	//tester+=1;
	return 0;
}
