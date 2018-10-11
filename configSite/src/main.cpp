/**
 * Create By José M. C. Noronha
 * Comando: g++ -Wall config.cpp -o config
 * nomestring.c_str() -> converte uma string em char c
 */
#include "../lib/ConfigApacheTool.h"

void executeOptionSelected(ConfigApacheTool &project, int opcao) {
    string porto, cmdToExec;

    // Consoante a opção do utilizador, a acção é realizada
    switch (opcao) {
        case 1:
            project.getInfoProject(true);
            break;
        case 2:
            project.getInfoProject(true);
            break;
        case 3:
            project.getInfoProject(false);

            // Pede o número do antigo porto
            cout << "Insert old port: ";
            project.oldPorto = project.getOlyInteger("\nInsert valid port: ");
            break;
        case 4:
            project.getInfoProject(true);
            project.disableProject();
            project.saveInfo(project.nomeProjecto, project.porto, true);
            break;
        case 5:
            // clear cin
            project.clearCin();

            // Pede o nome do projecto
            cout << "Insert project name: ";
            getline(cin, project.nomeProjecto);

            // Contruct command
            cmdToExec = "composer update -d " + project.pathWWW + "/" + project.nomeProjecto;
            system(cmdToExec.c_str());

            // Set permission on logs folder
            cmdToExec = project.commands.sudo + " " + project.commands.chmod + " -R 777 " + project.pathWWW + "/" + project.nomeProjecto;
            system(cmdToExec.c_str());
            break;
        case 6:
            project.getInfoProject(true);
            break;
        case 7:
            cmdToExec = "bash " + project.APPFOLDER + "infoListProject.sh" + " show";
            system(cmdToExec.c_str());
            break;
    }

    // Executa se a opcao não fôr sair
    if (opcao != 4 && opcao != 5 && opcao != 7 && opcao != 8) {
        project.executeAllNecessaryCommand(opcao);
        project.saveInfo(project.nomeProjecto, project.porto, false);
    }
}

/**
 * Main
 */
int main(){
	// Definições das vairiáveis
	ConfigApacheTool project;
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
        executeOptionSelected(project, opcao);

	  	// Change config for variable
	  	showMenu = true;
	  	opcao = -1;
	  	firstRun = false;
	}
  	
  	// exit
  	return 0;
}