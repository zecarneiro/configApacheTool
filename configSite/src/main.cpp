/**
 * Create By José M. C. Noronha
 * Comando: g++ -Wall config.cpp -o config
 * nomestring.c_str() -> converte uma string em char c
 */
#include "../lib/includes.h"
#include "../lib/ConfigSiteTool.h"

void executeTerminal() {
	system("x-terminal-emulator -e");
}

void selectServer(ConfigSiteTool &project) {
	bool firstShow = true;
	int option;
	do{
		if (firstShow) {
			project.printMenu(0);
			firstShow = false;
		}

		// Espera pela opção escolhida pelo utilizador
		option = project.getOlyInteger("\nInsert valid option: ");
		
		// Apresenta uma mensagem se o utilizador não introduzir uma opção correcta
		if(option < 1 || option > 3){
			cout << "\nInsert valid option only!!!"<<endl;
			cout<<"Insert an option: ";
		}
	}while (option < 1 || option > 3);

	if (option == 3) exit(0);
}

void selectFramework(ConfigSiteTool &project, bool &isBack) {
	bool firstShow = true;
	int option;
	do{
		if (firstShow) {
			project.printMenu(1);
			firstShow = false;
		}

		// Espera pela opção escolhida pelo utilizador
		option = project.getOlyInteger("\nInsert valid option: ");
		
		// Apresenta uma mensagem se o utilizador não introduzir uma opção correcta
		if(option < 1 || option > 4){
			cout << "\nInsert valid option only!!!"<<endl;
			cout<<"Insert an option: ";
		}
	}while (option < 1 || option > 4);

	if (option == 3) isBack = true;
	else if (option == 4) exit(0);
}

/**
 * Main
 */
int main(){
	// Definições das vairiáveis
	bool showMenu = true, firstRun=true, isBack = false;
	int  opcao;

	while(1) {
		ConfigSiteTool *project;
		project = new ConfigSiteTool();
		
		if (!firstRun){
			cout << "\n\n\n### Operation finished! ###\n" << endl;
		}

		do{
			// Se o menu não foi imprimido, então faz o print do mesmo
	    	if (showMenu) {
				selectServer(*project);
				selectFramework(*project, isBack);

				if (!isBack) project->printMenu(2);			
	    		showMenu = false;
	    	}

			if (!isBack) {
				// Espera pela opção escolhida pelo utilizador
	      		opcao = project->getOlyInteger("\nInsert valid option: ");
			} else {
				opcao = 8;
				isBack = false;
			}
	      	
	      	// Apresenta uma mensagem se o utilizador não introduzir uma opção correcta
	      	if(opcao < 1 || opcao > 9){
				cout << "\nInsert valid option only!!!"<<endl;
	  			cout<<"Insert an option: ";
	      	}
		}while (opcao < 1 || opcao > 9);

		// Exit
		if (opcao == 9) {
			break;
		}

		if (opcao != 8) {
			// Execute option selected
        	project->executeOptionSelected(opcao);
			firstRun = false;
		} else {
			system("clear");
			firstRun = true;
		}

	  	// Change config for variable
	  	showMenu = true;
	  	opcao = -1;
		delete project;
	}
  	
  	// exit
  	return 0;
}