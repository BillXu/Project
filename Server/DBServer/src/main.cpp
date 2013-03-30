#include"main.h"
#include <iostream>
#include "ThreadMod.h"
#include "mutex.h"
#include <my_global.h>
#include "mysql.h"
class A
	:public CThreadT
{
public:
	void __run()
	{
		printf("this function \n") ;
		//try 
		//{
		//	//TestFunc();
		//	A* a = NULL ;
		//	a->a = 3 ;
		//}
		//catch(...)
		//{
		//	return ;
		//	printf("error\n") ;
		//}
	}

	void TestFunc()
	{
	  MYSQL *conn; 
	  MYSQL_RES *result; 
	  MYSQL_ROW row;  
	  int num_fields; 
	  int i;  
	  conn = new MYSQL;
	  conn = mysql_init(NULL);  
	  if ( !mysql_real_connect(conn, "localhost", "root", "123456", "sakila", 3307, NULL, 0) )
	  {
		 fprintf(stderr, "Failed to connect to database: Error: %s\\n",         mysql_error(conn));
		printf("error\n") ;
		return ;
	  }
	  mysql_query(conn, "SELECT * FROM city"); 
	  result = mysql_store_result(conn); 
	  num_fields = mysql_num_fields(result); 
	  while ((row = mysql_fetch_row(result))) 
	  {    
			for(i = 0; i < num_fields; i++)      
			 {        
				 printf("%s ", row[i] ? row[i] : "NULL");      
			 }   
			 printf("\n"); 
	  } 
	  mysql_free_result(result);
	  mysql_close(conn);
	}

	int a ; 
};

int main()
{
	A a ;
	a.Start() ;
	a.TestFunc();
	//try 
	//{
	//	//TestFunc();
	//	A* a = NULL ;
	//	a->a = 3 ;
	//}
	//catch(...)
	//{
	//	printf(" expection error\n") ;
	//}
	getchar();
	return 0 ; 
}