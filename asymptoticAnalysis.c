/**
 *@file asymptotic_analysis.c
 *@author Capone, Della Rovere, Gortani, Fior
 *@brief stima dei tempi di esecuzione dei 3 diversi algoritmi di selezione
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include "quickSelect.c"
#include "heapSelect.c"
#include "medianMediansSelect.c"

// alloca 'number' spazio al vettore di tipo int
#define MALLOC_ARRAY(number, type)\
	((type*) malloc((number) *sizeof(type)))

FILE * fptr;
double Tmin = 0;

// costanti A, B distribuzione esponenziale
const int A = 100;
const double B = 0.157673137;

typedef enum {
	QuickSelect,
	HeapSelect,
	MedianMediansSelect
} Algorithm;

// definisce come devono essere popolati gli elementi in un array, utile per lo studio caso peggiore di quickselect
typedef enum {
	Random,
	Sort
} Generation;

// siccome i dati possono essere appesi in file diversi e dipende dall'analisi che sto effettuando:
typedef enum {
	AverageTimes,
	WorstCase
} Case;

double duration(struct timespec start, struct timespec end) {
	return end.tv_sec - start.tv_sec +
		((end.tv_nsec - start.tv_nsec) / (double) 1000000000.0);
}

double getResolution() {
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);
	do {
		clock_gettime(CLOCK_MONOTONIC, &end);
	} while (duration(start, end) == 0.0);
	return duration(start, end);
}

/**
 * @brief calcola il tempo minimo misurabile dalla macchina
 *        a partire dalla risoluzione stimata e dall'errore massimo ammissibile
 * @return double tempo minimo misurabile
*/
double getTmin() {
	double R = getResolution();
	double E = 0.001;
	return R *(1 / E + 1);
}

/**
 * @brief genera un numero casuale compreso tra -RAND_MAX e RAND_MAX 
 *        genero inizialmente un numero random tra 0 e 1 (sign)
 *        se sign è 0: moltiplico -1 per il numero random generato tra 0 ... RAND_MAX
 *        altrimenti: moltiplico 1 per numero random generato (non modificando quindi il segno)
 * @return int numero random compreso tra[-RAND_MAX ... RAND_MAX]
*/
int randomInteger() {
	int randomUnsigned = rand();
	int sign = rand() % 2;
  return ( sign == 0 ) ? ( -1 * randomUnsigned ) : randomUnsigned;
}

void populate(int A[], int len, Generation generationType) {
	if (generationType == Random)
		for (int i = 0; i < len; i++) A[i] = randomInteger();
	else
		for (int i = 0; i < len; i++) A[i] = i;
}

/**
 * @brief tempo d'esecuzione di un algoritmo di selezione su un campione
 * @param type algoritmo di selezione da utilizzare
 * @param A vettore interi pseudocasuali
 * @param size dimensione
 * @param k indice di selzione
 * @return double tempo (secondi)
*/
double executionTimeEvaluation( Algorithm type, int A[], int size, int k ) {
	double period = 0;
	struct timespec start, end;
	int kSmallest;

	clock_gettime(CLOCK_MONOTONIC, &start);
	do {
		switch (type)
		{
			case QuickSelect:
				kSmallest = quickSelect(A, 0, size - 1, k);
				break;
			case HeapSelect:
				kSmallest = heapSelect(A, 0, size - 1, k);
				break;
			case MedianMediansSelect:
				kSmallest = MoMSelect(A, k, 0, size - 1);
				break;
		}

		clock_gettime(CLOCK_MONOTONIC, &end);
		period = duration(start, end) - period;
	} while (period <= Tmin);

	return period;
}

/**
 * @brief appende una riga al file CSV asymptotic_times e scrive in console
 * @param type algoritmo di selezione
 * @param size dimensione del vettore o campione
 * @param average tempo (medio, in questo caso)
*/
void appendData( Algorithm type, Case which, int size, double average ) {
	if ( which == AverageTimes ) {
		switch (type) {
			case QuickSelect:
				fprintf(fptr, "quickSelect, %d, %f\n", size, average);
				printf("[appended CSV] quickSelect, size %d, average time %fs\n", size, average);
				break;

			case HeapSelect:
				fprintf(fptr, "heapSelect, %d, %f\n", size, average);
				printf("[appended CSV] heapSelect, size %d, average time %fs\n", size, average);
				break;

			case MedianMediansSelect:
				fprintf(fptr, "medianMediansSelect, %d, %f\n", size, average);
				printf("[appended CSV] medianMediansSelect, size %d, average time %fs\n", size, average);
				break;

			default:
				break;
		}
	} else {
		// file analisi caso peggiore di quickSelect
		fprintf(fptrWC, "quickSelect, %d, %f\n", size, average);
		printf("[appended CSV] quickSelect, size %d, average time %fs\n", size, average);
	}
}

/**
 * @brief calcola la dimensione del campione seguendo la distribuzione esponenziale 
 *        per un indice i compreso tra [0...99] e due costanti A, B (globali) t.c.
 *        n0 = 100, n99 = 5'000'000
 * @param i indice
 * @return int dimensione dei campioni
*/
int expDistribution(int i) {
	return A* pow(2, B *i);
}

/**
 * @brief applica 3 algoritmi di selezione
 *        su nSamples campioni (allocati dinamicamente) dimensione ni ciascuno
 *        calcola il tempo d'esecuzione medio
 * @param ni dimensione dei campioni
*/
void evaluateAverageOfSamples(int ni) {

	double quickSelectAvg = 0;
	double heapSelectAvg = 0;
	double medianSelectAvg = 0;
	int nSamples = 100;
	int k = 99;

	int *sample = NULL;
	for (int i = 1; i <= nSamples; i++)
	{
		sample = MALLOC_ARRAY(ni, int);
		populate(sample, ni, Random);

		// sommatoria tempi esecuzione
		quickSelectAvg += executionTimeEvaluation(QuickSelect, sample, ni, k);
		heapSelectAvg += executionTimeEvaluation(HeapSelect, sample, ni, k);
		medianSelectAvg += executionTimeEvaluation(MedianMediansSelect, sample, ni, k);

		free(sample);
	}

	// calcolo media tempo esecuzione da nSamples campioni
	quickSelectAvg /= nSamples;
	heapSelectAvg /= nSamples;
	medianSelectAvg /= nSamples;

	appendData(QuickSelect, AverageTimes, ni, quickSelectAvg);
	appendData(HeapSelect, AverageTimes, ni, heapSelectAvg);
	appendData(MedianMediansSelect, AverageTimes, ni, medianSelectAvg);
}

void evaluateAverageOfSamples_worstcase(int ni) {

	double quickSelectAvg = 0;
	int nSamples = 100;
	int k = 0;

	int *sample = NULL;
	for (int i = 1; i <= nSamples; i++)
	{
		sample = MALLOC_ARRAY(ni, int);
		populate(sample, ni, Sort);

		// sommatoria tempi esecuzione
		quickSelectAvg += executionTimeEvaluation(QuickSelect, sample, ni, k);

		free(sample);
	}

	// calcolo media tempo esecuzione da nSamples campioni
	quickSelectAvg /= nSamples;

	appendData(QuickSelect, WorstCase, ni, quickSelectAvg);
}

/**
 * @brief genera 100 dimensioni seguendo una distribuzione esponenziale
 */
void asymptoticTimes() {

	fptr = fopen("asymptotic_times.csv", "a");
	int ni;
	for (int i = 0; i <= 99; i++)
	{
		ni = expDistribution(i);
		evaluateAverageOfSamples(ni);
	}

	fclose(fptr);
}

void asymptoticTimes_worstcase() {

	fptr = fopen("asymptotic_times_wc.csv", "a");
	int ni;
	for (int i = 0; i <= 99; i++)
	{
		ni = expDistribution(i);
		evaluateAverageOfSamples_worstcase(ni);
	}

	fclose(fptr);
}

void standard_deviation () {

	FILE * fptrSTD = fopen("asymptotic_standard_deviation.csv", "w");
	fprintf(fptrSTD, "algorithm, stanard_deviation\n");
	// n fissato
	const int n = 100;
	int A[n];
	int k = 50;

	double quickSelectTimes[100], heapSelectTimes[100], medianSelectTimes[100];

	for (int i = 0; i < 100; i++)
	{
		populate(A, n, Random);
		// tempi esecuzione
		quickSelectTimes[i] = executionTimeEvaluation(QuickSelect, A, n, k);
		heapSelectTimes[i] = executionTimeEvaluation(HeapSelect, A, n, k);
		medianSelectTimes[i] = executionTimeEvaluation(MedianMediansSelect, A, n, k);
	}

	double quickSelectAverage = 0, heapSelectAverage = 0, medianSelectAverage = 0;
	for (int i = 0; i < 100; i++)
	{
		quickSelectAverage += quickSelectTimes[i];
		heapSelectAverage += heapSelectTimes[i];
		medianSelectAverage += medianSelectTimes[i];
	}

	quickSelectAverage /= 100;
	heapSelectAverage /= 100;
	medianSelectAverage /= 100;

	// calcolo standard deviation dati i tempi medi
	double quickSelectSTD = 0, heapSelectSTD = 0, medianSelectSTD = 0;
	for (int i = 0; i < 100; i++)
	{
		quickSelectSTD += pow((quickSelectTimes[i] - quickSelectAverage), 2);
		heapSelectSTD += pow((heapSelectTimes[i] - heapSelectAverage), 2);
		medianSelectSTD += pow((medianSelectTimes[i] - medianSelectAverage), 2);
	}

	quickSelectSTD = pow((quickSelectSTD / (n-1)), 0.5);
	heapSelectSTD = pow((heapSelectSTD / (n-1)), 0.5);
	medianSelectSTD = pow((medianSelectSTD / (n-1)), 0.5);


	
	fprintf(fptrSTD, "algorithm, stanard_deviation\n");
	fprintf(fptrSTD, "quickSelect, %f\n", quickSelectSTD);
	fprintf(fptrSTD, "heapSelect, %f\n", heapSelectSTD);
	fprintf(fptrSTD, "medianMediansSelect, %f\n", medianSelectSTD);
	fclose(fptrSTD);
}

int main() {
	struct timespec program_start, program_end;
	clock_gettime(CLOCK_MONOTONIC, &program_start);
	srand(time(NULL));
	Tmin = getTmin();

	fptr = fopen("asymptotic_times.csv", "w");
	fprintf(fptr, "algorithm, size, time\n");
	fclose(fptr);

	// analisi asintotica tempi medi 3 algoritmi di selezione
	asymptoticTimes();

	fptrWC = fopen("asymptotic_times_wc.csv", "w");
	fprintf(fptrWC, "algorithm, size, time\n");
	fclose(fptrWC);

	// analisi asintotica tempi medi quickselect caso peggiore
	asymptoticTimes_worstcase();

	// analisi deviazione standard
	standard_deviation();

	return (EXIT_SUCCESS);
}