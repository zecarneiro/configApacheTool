/**
 * Create By José M. C. Noronha
 * Comando: g++ -Wall config.cpp -o config
 * nomestring.c_str() -> converte uma string em char c
 */
#include "../lib/includes.h"
#include "../lib/ConfigSiteTool.h"

/**
 * Main
 */
int main(){
	// Definições das vairiáveis
	ConfigSiteTool project;
	bool showMenu = true, firstRun=true;
	int  opcao;

	while(1) {
		if (!firstRun){
			cout << "\n\n\n### Operation finished! ###\n" << endl;
		}

		do{
			// Se o menu não foi imprimido, então faz o print do mesmo
	    	if (showMenu) {
	    		project.printMenu();
	    		showMenu = false;
	    	}
	    	// Espera pela opção escolhida pelo utilizador
	      	opcao = project.getOlyInteger("\nInsert valid option: ");
	      	
	      	// Apresenta uma mensagem se o utilizador não introduzir uma opção correcta
	      	if(opcao < 1 || opcao > 8){
				cout << "\nInsert valid option only!!!"<<endl;
	  			cout<<"Insert an option: ";
	      	}
		}while (opcao < 1 || opcao > 8);

		// Exit
		if (opcao == 8) {
			break;
		}

        // Execute option selected
        //executeOptionSelected(project, opcao);

	  	// Change config for variable
	  	showMenu = true;
	  	opcao = -1;
	  	firstRun = false;
	}
  	
  	// exit
  	return 0;
}