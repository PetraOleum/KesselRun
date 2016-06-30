#include "kesselapp.h"

int main(int argc, char* argv[]) {
	KesselApp app;
	
	for (int i = 1; i < argc; i++)
		printf("%d: %s\n", i, argv[i]);

	return app.OnExecute();
}
