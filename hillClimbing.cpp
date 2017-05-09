#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <time.h>
#include <fstream>

using namespace std;

vector<int> read_file(ifstream &file, vector< vector<int> >& phi);

bool DPLL(vector< vector<int> > phi, vector<int>& variables);

void simplify(vector< vector<int> >& phi, int x);

int choose_literal(vector<int>& variables);

vector<int> walkSAT(vector< vector<int> >& , vector<int>, vector<int>, int);

void randomAssignment(vector<int>&);

int pick_literal(vector<int>& variables, vector<int> clause);

vector< vector<int> > evaluate(vector<int>, vector< vector<int> >);

vector<int> hillClimbing(vector< vector<int> >& , vector<int> , vector<int>, int);

int main(int argc,char *argv[]){

	// initialize the seed for the rand function

	srand(time(0));

	vector< vector<int> > formula;

	ifstream fin;  //file object

 	fin.open(argv[1]); //open the file

	int flips = atoi(argv[2]);

//	fin.open("./sat/easy/3.cnf");

	if(!fin)

		cout<<"error: no input file"<<endl;

	else{

		// this vector stores the number 
		// of occurrences of each variable in the formula.
		// The index represents the literal and each 
		// element is the number of occurrences.

		vector<int> occurrences = read_file(fin, formula);  // Read the data in the file into a vector.
	
		// the above function reads the empty line in 
		// the end of the file, which stores one more 
		// vector than the actual number of clauses
		// so this line of code deletes the empty clause vector.

		formula.pop_back();

		// this vector stores the current assignment.

		vector<int> assignment;

                for(int i = 0; i < occurrences.size(); i ++)

                        assignment.push_back(0);

		randomAssignment(assignment);

		//assignment = walkSAT(formula, assignment, occurrences, flips);

		assignment = hillClimbing(formula, assignment, occurrences, flips);

		for(int j = 1; j < assignment.size(); j ++)

			cout<<assignment[j]<<' ';

		cout<<endl;

		/*bool satisfiable = DPLL(formula, occurrences);

		if(satisfiable) cout<<"Satisfiable"<<endl;
		else	cout<<"Not Satisfiable"<<endl;
		*/
	}
	return 0;
}

vector<int> hillClimbing(vector< vector<int> >& phi, vector<int> assignment, vector<int> variables, int max){

	// this vector stores the 
	// unsatisfied clauses.

	vector< vector<int> > unsatisfied;

	// store the occurrences array into 
	//temporary vector.

	vector<int> temp = variables;
	
	// the current chosen literal

	int literal;

	// the last chosen literal

	int lastLiteral;

	// generate a random assignment

	randomAssignment(assignment);

	// substitute the boolean values
	// into the formula and store the
	// unsatisfied clauses.

	unsatisfied = evaluate(assignment, phi);

	// while the formula is not satisfied:

	int i = 0;

	while(!unsatisfied.empty() && i < max){

		// choose the literal that 
		// maximally improves the quality 
		// of the resulting assignment

		literal = choose_literal(temp);

		// if we reach a plateau, randomly restart.

		if(lastLiteral == literal){

                        	temp = variables;

                        	randomAssignment(assignment);

                        	unsatisfied = evaluate(assignment, phi);

             	}

		// otherwise, flip the chosen bit
		// and evaluate the formula.
	
		else{

			if(assignment[literal] == 0)

                       		assignment[literal] = 1;
                	else
                        	assignment[literal] = 0;

			unsatisfied = evaluate(assignment, phi);

			lastLiteral = literal;
		}

		i++;
	}
	if(unsatisfied.empty())

		cout<<"success! Satisfying Assignment: ";
	else
		cout<<"No solution! Best assignment found: ";

	return assignment;
}

vector<int> walkSAT(vector< vector<int> >& phi, vector<int> assignment, vector<int> variables, int maxFlips){

	// this variable stores 
	// the last chosen clause 
	// to prevent the program
	// from choosing it two 
	// times in a row.

	int lastIndex = 0;

	int index = 0;

	cout<<"Assignment:"<<endl;

	for(int j = 1; j < assignment.size(); j ++){

		cout<<assignment[j]<<endl;
	}

	// substitute the initial assignment.

	vector< vector<int> > unsatisfied = evaluate(assignment, phi);

	int i = 0;

	// while not satisfied:

	while(!unsatisfied.empty() && i < maxFlips){

		// calculate the probability

		int num = rand() % 100 + 1;

                float probability = num/100.00;
	
		int literal;

		// with probability 1-p, choose 
		// a clause and then choose 
		// a literal that minimizes 
		// the number of unsatisfied 
		// when flipped. 

		if(probability > 0.50){

			while(index == lastIndex)

				if(unsatisfied.size() - 1 != 0)

                        		index = rand() % (unsatisfied.size() - 1);

				else
					index = 1;


			literal = pick_literal(variables, unsatisfied[index]);	
		}
	
		// else, with probability p,
		// randomly choose a literal and flip it.

		else{
			literal = rand() % (variables.size() - 1) + 1;

		}	

		// flip the bit

		if(assignment[literal] == 0)

			assignment[literal] = 1;
		else
			assignment[literal] = 0;


		// evaluate the formula

		unsatisfied = evaluate(assignment, phi);

		// store the last chosen clause

		lastIndex = index;

		i ++;
	}
	if(unsatisfied.empty())

		cout<<"works, satisfying assignment:"<<endl;

	else

		cout<<" Could not find solution, best assignment found:"<<endl;

	return assignment;
}

int pick_literal(vector<int>& variables, vector<int> clause){

        // find the literal with the 
        // highest number of occurrences
        // and set its number of occurrences
        // to 0, to avoid picking it again.

	vector<int> temp = variables;

        int max;

        int literal = 0;

	bool found = false;	

	while(!found){

		max = *max_element(temp.begin(), temp.end());

		vector<int>::iterator it = search_n (temp.begin(), temp.end(), 1, max);

		literal = it - temp.begin();

		temp[literal] = 0;

		// make sure the literal is in the chosen clause

		for(int j = 0; j <= clause.size() - 1; j++){

			if(clause[j] == literal || clause[j] == (-1 * literal))

				found = true;
		}
	}

        return literal;
}

void randomAssignment(vector<int>& values){

	// generate an array of 0's and 1's
	// and randomly shuffle it.

	for(int i = 0; i < values.size(); i++){

		values[i] = rand() % 2;
	}
	random_shuffle(values.begin(), values.end());
}

vector< vector<int> > evaluate(vector<int> assignment, vector< vector<int> > phi){

	// store the unsatisfied clauses

	vector< vector<int> > unsatisfiedClauses;

	// for each clause:

	for(int i = 0; i <= (phi.size() - 1); i++){

		vector<int> eval;

		// obtain the value of the literal
		// if it's negated flip its value.

		for(int j = 0; j < phi[i].size(); j++){

			int temp = phi[i].at(j);

			if(abs(temp) == (temp * -1))

				if(assignment[abs(temp)] == 0)
		
					assignment[abs(temp)] = 1;
				else
					assignment[abs(temp)] = 0;
			else{}

			eval.push_back(assignment[abs(temp)]);
		}
		int E = 0;

		// or each two literals
		// if the result is true
		// increment E.

		for(int k = 0; k < eval.size() - 1; k++){
		
			if(eval[k] | eval[k+1])
				
				E++;
			else{}
		}

		// if the clause yields false, store 
		// it in the unsatisfied clauses vector.

		if(E == 0)
			unsatisfiedClauses.push_back(phi[i]);
		else{}
	}
	return unsatisfiedClauses;
}

bool DPLL(vector< vector<int> > phi, vector<int>& variables){

	bool satisfiable;

	// If the formula has no clauses, return true. 

	if(phi.empty()){

		satisfiable = true;

		return satisfiable;
	}
	// If the formula has an empty clause, return false.

		for(int i = 0; i <= (phi.size() - 1); i++){

			if(phi[i].empty()){

				satisfiable = false;
				
				return satisfiable;
			} 
		}

		// If it contains a unit clause, 
		// remove clauses where the literal appears
		// and remove the literal from all clauses
		// where it appears negated.

		for(int i = 0; i <= (phi.size() - 1); i++){

			if(phi[i].size() == 1){

				vector< vector<int> > temp = phi;

				simplify(temp, phi[i].at(0));

				satisfiable = DPLL(temp, variables);

				return satisfiable;
			}
		}

		// if there are no unit clauses, pick 
		// the variable with the highest number 
		// of occurrences, set it to true
		// and see if it satisfies the formula
		// if not set it to false and substitute it
		// into the formula.

		int literal = choose_literal(variables);

		if(literal == 0){

			satisfiable = false;

			return satisfiable;
		}
		else{
		vector< vector<int> > temp1 = phi;

		simplify(temp1, literal);

		if(DPLL(temp1, variables)){

			satisfiable = true;

			return satisfiable;
		}
		else{
			vector< vector<int> > temp2 = phi;

			simplify(temp2, (literal * -1));
		
			satisfiable = DPLL(temp2, variables);

			return satisfiable;
		}
		}
}

int choose_literal(vector<int>& variables){

	// find the literal with the 
	// highest number of occurrences
	// and set its number of occurrences
	// to 0, to avoid picking it again.

	int max = 0;

	int literal = 0;

	for(int i = 0; i < variables.size(); i++){

		if(max < variables[i]){

			max = variables[i];
		
			literal = i;
		}
	}
	variables[literal] = 0;

	if(max == 0){

		literal = 0;
		
	//	literal = rand() % (variables.size() - 1) + 1;

	}
	return literal;
}

void simplify(vector< vector<int> >& phi, int x){

	for(int i = 0; i <= (phi.size() - 1); i++){
		
		int j = 0;

		while(j <= (phi[i].size() - 1) && !phi[i].empty()){

			if(phi[i].at(j) == x){

// remove clauses where x appears
				phi.erase(phi.begin()+i);

				break;
			}
// remove -x from the other clauses
			else if(phi[i].at(j) == (x * -1)){


				phi[i].erase((phi[i].begin())+j);

				break;
			}
			else{}
			j++;
		}
		if(phi.empty())

			break;
	}
}

vector<int> read_file(ifstream &file, vector< vector<int> >& phi){

	int numOfVariables = 0;

	string skipped;

	int literal;

	int index;

	// read the number of variables
	// and ignore the rest of the first line in 
	// the cnf file.

	for(int i = 0; i <= 1; i++)

		file >> skipped;

	file >> numOfVariables;

	file >> skipped;

	// This array stores the number of 
	// occurrences of each variable.

	vector<int> variables;

	for(int i = 0; i < numOfVariables + 1; i++)

		variables.push_back(0);

	while(file){

		vector<int> clause;

		file >> literal;

		while(literal != 0){

			clause.push_back(literal);

			index = abs(literal);

			variables.at(index)++;

			file >> literal;
		}	
		phi.push_back(clause);
	}
	return variables;
}
