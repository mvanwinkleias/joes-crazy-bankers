// This file is almost directly from:
// https://begriffs.com/posts/2020-03-23-concurrent-programming.html
//
// I wanted to experiment with the GNU build system, and see
// how to link against libraries, etc.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define N_ACCOUNTS 50
#define N_THREADS 100
#define N_ROUNDS 10000

#define INIT_BALANCE 100

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) < (b) ? (b) : (a))

struct account
{
	long balance;
	pthread_mutex_t mtx;
} accts[N_ACCOUNTS];

int rand_range(int N)
{
	return (int)((double)rand() / ((double)RAND_MAX +1) * N);
}

pthread_mutex_t stats_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t stats_cnd = PTHREAD_COND_INITIALIZER;

int stats_curr = 0;
int stats_best = 0;

void stats_change(int delta)
{
	pthread_mutex_lock(&stats_mtx);
	stats_curr += delta;
	if (stats_curr > stats_best)
	{
		stats_best = stats_curr;
		pthread_cond_broadcast(&stats_cnd);
	}
	pthread_mutex_unlock(&stats_mtx);
}

void *stats_print(void *arg)
{
	int prev_best;
	
	(void)arg;
	pthread_detach(pthread_self());
	
	while (1)
	{
		pthread_mutex_lock(&stats_mtx);
		
		prev_best = stats_best;
		while (prev_best == stats_best)
		{
			pthread_cond_wait(
				&stats_cnd,
				&stats_mtx
			);
		}
		
		printf("\r%2d", stats_best);
		pthread_mutex_unlock(&stats_mtx);
		
		fflush(stdout);
	}
}


void *disburse(void *arg)
{
	size_t i, from, to;
	long payment;
	
	(void)arg;
	
	for (i = 0; i < N_ROUNDS; i++)
	{
		from = rand_range(N_ACCOUNTS);
		
		do {
			to = rand_range(N_ACCOUNTS);
		} while (to == from);
		
		pthread_mutex_lock(&accts[MIN(from,to)].mtx);
		pthread_mutex_lock(&accts[MAX(from,to)].mtx);
		
		stats_change(1);
		
		if (accts[from].balance > 0)
		{
			payment = 1 + rand_range(accts[from].balance);
			accts[from].balance -= payment;
			accts[to].balance += payment;
		}
		
		stats_change(-1);
		
		pthread_mutex_unlock(&accts[MAX(from,to)].mtx);
		pthread_mutex_unlock(&accts[MIN(from,to)].mtx);
		
	}
	return NULL;
}

int main(void)
{
	size_t i; 
	long total;
	
	pthread_t
		ts[N_THREADS],
		stats;
	
	srand(time(NULL));
	
	for (i = 0; i < N_ACCOUNTS; i++)
	{
		accts[i] = (struct account) {
			.balance = INIT_BALANCE,
			.mtx = PTHREAD_MUTEX_INITIALIZER,
		};
	}
	
	printf(
		"Initial money in system: %d\n",
		N_ACCOUNTS * INIT_BALANCE
	);
	
	for (i = 0; i < N_THREADS; i++)
		pthread_create(&ts[i], NULL, disburse, NULL);
	
	pthread_create(&stats, NULL, stats_print, NULL);
	
	for (i = 0; i < N_THREADS; i++)
		pthread_join(ts[i], NULL);
	
	for (total = 0, i = 0; i < N_ACCOUNTS; i++)
		total += accts[i].balance;
	
	printf(
		"\nFinal money in system: %ld\n",
		total
	);
	
}
