#include <stdio.h>

int main() {
	printf("ml' nob:\n");
	int number;
	char extra;
	const char enter  = '\n';
	if (scanf("%d%c", &number, &extra) !=2) {
		printf("Neh mi\'\n");
		return 1;
	} else if (number<0  || number >8){
		printf("Qih mi' %d\n", number);
		return 2;
	} else if (extra != enter && extra != ' ') {
		printf("bIjatlh 'e' yImev\n");
		return 3;
	} else if (number == 0){
		printf("Qapla'\nnoH QapmeH wo' Qaw'lu'chugh yay chavbe'lu' 'ej wo' choqmeH may' DoHlu'chugh lujbe'lu'.\n");
	} else if (number == 1){
		printf("Qapla'\nbortaS bIr jablu'DI' reH QaQqu' nay'.\n");
	} else if (number == 2){
		printf("Qapla'\nQu' buSHa'chugh SuvwI', batlhHa' vangchugh, qoj matlhHa'chugh, pagh ghaH SuvwI''e'.\n");
	} else if (number == 3){
		printf("Qapla'\nbISeH'eghlaH'be'chugh latlh Dara'laH'be'.\n");
	} else if (number == 4){
		printf("Qapla'\nqaStaHvIS wa' ram loS SaD Hugh SIjlaH qetbogh loD.\n");
	} else if (number == 5){
		printf("Qapla'\nSuvlu'taHvIS yapbe' HoS neH.\n");
	} else if (number == 6){
		printf("Qapla'\nHa'DIbaH DaSop 'e' DaHechbe'chugh yIHoHQo'.\n");
	} else if (number == 7){
		printf("Qapla'\nHeghlu'meH QaQ jajvam.\n");
	} else if (number == 8){
		printf("Qapla'\nleghlaHchu'be'chugh mIn lo'laHbe' taj jej.\n");
	};
	return 0;
}
