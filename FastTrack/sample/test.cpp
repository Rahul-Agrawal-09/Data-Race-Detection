#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "./vector_clock.cc"
using namespace std;

int tester; //shared variable
VectorClock vc_main; //parent thread vector clock
pthread_t parent_id; //parent thread id

//vector clock of the thread that last writes the tester 
//at time when tester is last written  
VectorClock tester_write; 

void *thread1(void *arg)
{
	cout << "T1 Created." << endl;
	pthread_t my_id=pthread_self();
	VectorClock vc=vc_main;

	//inheriting the vector clock of the parent thread
	vc.SetClock(my_id,vc_main.GetClock(parent_id));

	//check if T1 happens before than write tester
	bool ans=tester_write.HappensBefore(&vc);
	cout << "Can T1 Happens Before Last Written: " << ans << endl;

	//if T1 Happens Before write tester then update tester_write vector clock
	if(ans){
		tester_write=vc;
		tester += 3;
	}
	//if T1 do not Happens Before tester_writer than report data race in T1
	else{
		cout<<"!----Data Race in T1----!\n"<<endl;
	}
	vc.Increment(my_id); //incremented vector clock

	//Some delay in T1
	for(int i=0;i<400000;i++)
		vc.Increment(my_id); //incremented vector clock

	vc.Increment(my_id); //incremented vector clock
	cout<<"Tester in T1 : "<<tester<<endl;
	vc.Increment(my_id); //incremented vector clock

	//joining thread 1 vector clock with parent thread vector. 
	vc_main.Join(&vc);
	cout << "T1 Ended." << endl;
	return NULL;
}


void *thread2(void *arg)
{
	cout << "T2 Created."<< endl;
	pthread_t my_id=pthread_self();
	VectorClock vc=vc_main;

	//inheriting the vector clock of the parent thread
	vc.SetClock(my_id,vc_main.GetClock(parent_id));

	vc.Increment(my_id); //incremented vector clock
	for(int i=0;i<500000;i++) 
		vc.Increment(my_id); //incremented vector clock

	//check if T2 happens before than write tester
	bool ans=tester_write.HappensBefore(&vc);
	cout << "Can T2 Happens Before Last Written: " << ans << endl;

	//if T2 Happens Before write tester then update tester_write vector clock
	if(ans){
		tester_write=vc;
		tester += 3;
	}
	//if T2 do not Happens Before tester_writer than report data race in T2
	else{
		cout << "\n!----Data Race in T2----!\n"<<endl;
	}
	vc.Increment(my_id); //incremented vector clock
	cout<<"Tester in T2 : "<<tester<<endl;
	vc.Increment(my_id); //incremented vector clock

	//joining thread 2 vector clock with parent thread vector. 
	vc_main.Join(&vc);
	cout << "T2 Ended." << endl;
	return NULL;
}


void threadCretator()
{
	cout << "Test Program Started." << endl;
	pthread_t my_id=pthread_self();
	parent_id=my_id;
	vc_main.Increment(my_id); //incremented vector clock
	cout << my_id << endl;
	tester_write=vc_main;
	tester = 0;
	vc_main.Increment(my_id); //incremented vector clock

	vc_main.Increment(my_id); //incremented vector clock
	pthread_t id1,id2;
	vc_main.Increment(my_id); //incremented vector 
	
	//creating thread 1
	pthread_create(&id1,NULL,thread1,NULL);
	vc_main.Increment(my_id); //incremented vector clock

	//checking if main thread Happens Before tester_write
	bool ans=tester_write.HappensBefore(&vc_main);
	cout << "Can T2 Happens Before Last Written: " << ans << endl;
	//if Main Happens Before write tester then update tester_write vector clock
	if(ans){
		tester_write=vc_main;
		tester += 2;
	}
	//if Main do not Happens Before tester_writer than report data race in Main
	else{
		cout << "\n!----Data Race in Main----!"<<endl;
	}
	vc_main.Increment(my_id); //incremented vector clock

	//creating thread 2
	pthread_create(&id2,NULL,thread2,NULL);
	vc_main.Increment(my_id); //incremented vector clock

	//waiting for the threads to join
	pthread_join(id1,NULL);
	pthread_join(id2,NULL);

	cout<<"Tester in Main : "<<tester<<endl;
	vc_main.Increment(my_id); //incremented vector clock

	//printing tha final vector clock of the main thread
	cout << "Vector Clock of the Main Thread: " << vc_main.ToString() << endl;
	cout << "Test Program Ended." << endl;
}

int main()
{
	tester=0;
	threadCretator();
	return 0;
}
