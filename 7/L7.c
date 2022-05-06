#include <pthread.h>
#include <stdio.h>

char alphabet[]="0123456789abcdefghijk";

void thread(void *arg) // функция контролирующая работу нити
{
	int k, j;
	if((int)arg == 1 || (int)arg == 3) {
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE , NULL);// отклоняем запрос на завершение потока
	}
	for (k=0; k<21; k++) {
		printf(k+1, 2 * (int)arg, 5 - (int)arg);
		for (j=0; j<(int)arg * 2; j++) {
			printf("%c",alphabet[k]);//вывод буквы
		}
		printf("\n");

		if(k == 12 && (int)arg == 3) {
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE , NULL); //Третья нить на 13 шаге разрешает отмену
		pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);// запрос на завершение откладывается до следующей точки завершения
		}
		if(k == 15 && (int)arg == 3) {
			pthread_testcancel();
			usleep(1000000 + 750000 * (int)arg);//задержка выполнения в микросекундах
			pthread_testcancel(); //Точку отмены эта нить устанавливает между 16 и 17
		}
		else {
		usleep(1000000 + 500000 * (3 - (int)arg));
		}
	}
}

void main()
{

	int k;
	int rc;
	pthread_t tid1, tid2, tid3;

	printf("\033[2J\n");
	pthread_create(&tid1, NULL, (void*)thread, (void*)1); //три нити
	pthread_create(&tid2, NULL, (void*)thread, (void*)2);
	pthread_create(&tid3, NULL, (void*)thread, (void*)3);

	for (k=0; k<=20; k++) {
		printf(k+1);
		printf("%c\n",alphabet[k]);//вывод главной нити
		if(k == 5) {//на 5 шаге
			rc=pthread_cancel(tid1); //главная нить делает попытку отмены первой из дополнительных нитей
			if(rc != 0) {
				printf("Error: first thread  cancel failed");
			}
			else {
			printf("Trying to cancel first thread");
			}
		}
		if(k == 10) {
			rc=pthread_cancel(tid3); //делает попытку отмены третьей из дополнительных нитей.
			if(rc != 0) {
				printf("Error: third thread cancel failed");
			}
			else {
				printf("Third thread cancel is successfull");
			}
		}
		usleep(1500000);
	}
}
