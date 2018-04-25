
bool Control(int choise,struct Node* current,int level,int child, bool loop) {
	switch (choise) {
		case 1: 
			PrintNode(current);
			break;
		case 2:
			AddData(current);
			break;
		case 3:
			printf("Node has %d children\n",LeafsNumber(current));
			break;
		case 4: 
			AddChild(current);
			break;
		case 5:	{
			if (current->num>0) {
				int pos = WhichChild(current);
				TerminateTree(current->pointer[pos]);
				for (uint8_t j=pos;j<(current->num-1);j++)
					current->pointer[pos]=current->pointer[pos+1];
				--current->num;
				struct Node** tmp=(struct Node**)realloc(current->pointer,(current->num)*sizeof(struct Node*));
				if (!tmp)
					printf("Error in memory alocation\n");
			}
			else
				printf("There is no children, operation terminated\n");
			break;
		}
		case 6: {
			if (current->num>0) {
				current=current->pointer[WhichChild(current)];
			level++;
			}
			else
				printf("No children\n");
			break;
		}
		case 7: {
			if (current!=root) {
				current = LevelUp(current,root);
				level--;
				PrintNode(current);
			}
			else 
				printf("You in root\n");
			break;
		}
		case 8:
			if (current->num>0)
				GrowLeaf(current);
			else
				printf("no children\n");
			break;
		case 9: 
			PrintTree(root);
			printf("\n");
			break;
		case 10: 
			TerminateTree(root);
			Door();
			break;
		case 11: 
			SaveTree();
			break;
		case 12: 
			SaveTree();
			TerminateTree(root);
			loop=true;
			break;
		case 13:
			TerminateTree(root);
			loop=true; 	
			break;		
	}	
return loop;

}



int main () {

	printf("%s Tree creator \n Version %d \n",signature,version);
	root =Door();
	current=root;
	int level=0;	// level of the depth of the tree 
	int child=0;	// number of the child of previous node
	bool loop=false;
	printf("\033[H\033[J");
	while (!loop){
		printf("-------------------------------------\n");
		if ((level==0)&&(child==0))
			printf("You in root\n");
		else
			printf("You in level - %d child - %d\n",level,child);
		printf("Menu\n");
		printf("Enter option\n");
		printf("- Print data of curent node - 1\n");
		printf("- Change/Add data into curent node - 2\n");
		printf("- Number of children - 3\n");
		printf("- Add child - 4\n");
		printf("- Remove child - 5\n");
		printf("- Move to a child - 6\n");
		printf("- Move up - 7\n");
		printf("- Insert child - 8\n");
		printf("- Print all tree - 9\n");
		printf("- Load or Create new tree - 10 \n");
		printf("- Save - 11\n");
		printf("- Save & Exit - 12\n");
		printf("- Exit - 13\n");
		printf("-------------------------------------\n");
		int choise=0;
		while((choise<1)||(choise>13)) {
			scanf("%d",&choise);
			if ((choise<1)||(choise>13))
				printf("wrong input\n");
		}
		printf("\033[H\033[J");
		loop=Control(choise,current,level,child,loop);
		printf("----\n");	
	}
	
	return 0;
}	
