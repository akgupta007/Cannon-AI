#include <iostream>
#include <bits/stdc++.h>

// player_id = 0 is me.
int player_id;
#include "state.cpp"

using namespace std;

int counter = 0;
typedef vector<vector<int>> matrix;


string mv = "";
int check = 0;
int my_id = 0;

vector<string> getAllNeighbour(matrix* state){
	vector<string> neighbour;
	int search_value;

	matrix allSoldier = find_soldiers(state);
	vector<matrix> allCannons= find_cannons(allSoldier);
	for(int i=0; i<allSoldier.size(); i++){
		vector<int> moves = soldier_possible_moves(state, allSoldier[i]);
		for(int j=0; j<moves.size()/2; j++){
			string s ="S " + to_string(allSoldier[i][0]) + " " + to_string(allSoldier[i][1]) + " M " + to_string(moves[2*j]) + " " + to_string(moves[2*j+1]);
			neighbour.push_back(s);
		}

	}

	for(int i=0; i<allCannons.size(); i++){
		vector<string> bCannon= bomb_cannon(state, allCannons[i]);

		vector<string> mCannon = move_cannon(state, allCannons[i]);

		neighbour.insert(neighbour.end(), bCannon.begin(), bCannon.end());
		neighbour.insert(neighbour.end(), bCannon.begin(), bCannon.end());
	}

	return neighbour;
}



//assuming i am player 0
int evaluation_function(matrix* state, int p_id){
	int value = 0;
	int damage = 0;
	int N = (*state).size();
	int M = (*state)[0].size();


	int wsafety = 1;
	int wcannon = 1;
	int wtombsafety = 1;
	int wOpponentPlayers = 1;

	//number of soldiers and towns
	for(int i=0; i<(*state).size(); i++){
		for(int j=0; j<(*state)[i].size(); j++){
			if((*state)[i][j] == 3) value += 1;
			if((*state)[i][j] == 1) value -= 1;
			if((*state)[i][j] == 4) value += 30;
			if((*state)[i][j] == 2) value -= 30;
		}
	}

	// primary concern: opponent's damage or my safety
	matrix security_weightage(N, vector<int>(M, 1));
	matrix strategic_factor(N, vector<int>(M, 0));   // may not be needed
	matrix safety_factor(N, vector<int>(M, 0));     // partially done
	matrix attacking_factor(N, vector<int>(M, 0));   // partially done
	matrix value_added(N, vector<int>(M, 0));     
	matrix free_space(N, vector<int>(M, 0));    // partially done
	vector<matrix> direction = {{{1,1}, {0,1}, {-1,1}, {1,0}, {-1,0}},  {{1,-1}, {0,-1}, {-1,-1}, {1,0}, {-1,0}}};
	vector<matrix> fwd_direction = {{{1,-1}, {0,-1}, {-1,-1}},  {{1,1}, {0,1}, {-1,1}}};

	int check_value;
	int dir_id;
	for(int i=0; i<N; i++){
		for(int j=0; j<M; j++){
			if((*state)[i][j] == 3) {check_value = 3; dir_id = 0;}
			else if((*state)[i][j] == 1) {check_value = 1; dir_id = 1;}
			else continue;
			for(int k=0; k<direction[dir_id].size(); k++){
				vector<int> check_pos = {j+direction[dir_id][k][0], i+direction[dir_id][k][1]};
				if(valid_pos(&check_pos)){
					if ((*state)[check_pos[1]][check_pos[0]] == check_value) safety_factor[i][j] += 1;
				}
			}
			for(int k=0; k<fwd_direction[dir_id].size(); k++){
				vector<int> check_pos = {j+direction[dir_id][k][0], i+direction[dir_id][k][1]};
				if(valid_pos(&check_pos)){
					if ((*state)[check_pos[1]][check_pos[0]] == 0) {   // what if opponent is present
						if(dir_id == 0){
							free_space[check_pos[1]][check_pos[0]] += 1;
						}
						else{
							free_space[check_pos[1]][check_pos[0]] -= 1;					
						}
					}
				}
			}

			if((*state)[i][j] == 3) {check_value = 1; dir_id = 1;}
			if((*state)[i][j] == 1) {check_value = 3; dir_id = 0;}
			for(int k=0; k<direction[dir_id].size(); k++){
				vector<int> check_pos = {j+direction[dir_id][k][0], i+direction[dir_id][k][1]};
				if(valid_pos(&check_pos)){
					if ((*state)[check_pos[1]][check_pos[0]] == check_value) attacking_factor[i][j] += 1;
				}
			}
		}
	}

	//cannons attacking position
	matrix allSoldier = find_soldiers(state, 0);
	vector<matrix> cannonsP0 = find_cannons(allSoldier);
	int count = cannonsP0.size();
	value += count;
	for(int i=0; i<cannonsP0.size(); i++){
		vector<string> s = bomb_cannon(state, cannonsP0[i]);
		for(int j=0; j<s.size(); j++){
			int x = s[j][8] - '0';
			int y = s[j][10] - '0';
			vector<int> pos = {x, y};
			if((*state)[y][x] == 1) attacking_factor[y][x] = INT_MAX;
			if((*state)[y][x] == 2) value += 10;
			if(free_space[y][x] < 0) free_space[y][x] = 0;
		}
	}
	allSoldier = find_soldiers(state, 1);
	vector<matrix> cannonsP1 = find_cannons(allSoldier);
	value -= count;
	count = cannonsP1.size();
	for(int i=0; i<cannonsP1.size(); i++){
		vector<string> s = bomb_cannon(state, cannonsP1[i]);
		for(int j=0; j<s.size(); j++){
			int x = s[j][8] - '0';
			int y = s[j][10] - '0';
			if((*state)[y][x] == 3) attacking_factor[y][x] = INT_MAX;
			if((*state)[y][x] == 4) value -= 10;
			if(free_space[y][x] > 0) free_space[y][x] = 0;
		}
	}


	//cannons under threat
	for(int i=0; i<cannonsP0.size(); i++){
		for(int j=0; j<cannonsP0[i].size(); j++){
			int x = cannonsP0[i][j][0];
			int y = cannonsP0[i][j][1];
			if(attacking_factor[y][x] > 0) { value -= 3; if(p_id == 0){ damage = max(damage, 3 + attacking_factor[y][x]); }}
		}
	}
	for(int i=0; i<cannonsP1.size(); i++){
		for(int j=0; j<cannonsP1[i].size(); j++){
			int x = cannonsP1[i][j][0];
			int y = cannonsP1[i][j][1];
			if(attacking_factor[y][x] > 0) { value += 3; if(p_id == 1){ damage = min(damage, -3 - attacking_factor[y][x]); }}
		}
	}

	// towns under threat
	fwd_direction = {{{1,-1}, {1, -2}, {0,-1}, {0,-2}, {-1,-1}, {-1,-2}, {-2, -2}, {2, -2}},
					{{1,1}, {1, 2}, {0,1}, {0,2}, {-1,1}, {-1,2}, {-2, 2}, {2, 2}}};
	for(int i=0 ; i<N; i++){
		for(int j=0; j<M; j++){
			if( (*state)[i][j] == 4 ){
				for(int k = 0; k<fwd_direction[0].size(); k++){
					vector<int> pos = {j + fwd_direction[0][k][0],i + fwd_direction[0][k][1]};
					if(valid_pos(&pos) && free_space[pos[1]][pos[0]] < 0){
						value -= 5;
					}
				}
			}
			if( (*state)[i][j] == 2 ){
				for(int k = 0; k<fwd_direction[1].size(); k++){
					vector<int> pos = {j + fwd_direction[1][k][0],i + fwd_direction[1][k][1]};
					if(valid_pos(&pos) && free_space[pos[1]][pos[0]] > 0){
						value += 5;
					}
				}
			}
		}
	}

	for(int i=0 ; i<N; i++){
		for(int j=0; j<M; j++){
			if((*state)[j][i] == 3){
				if(attacking_factor[j][i] > 0){
						if(attacking_factor[j][i] == INT_MAX) value -= 2;
						else value += safety_factor[j][i] - attacking_factor[j][i];
						if(p_id == 0) damage = max(damage, 1);
					}
			}
			else if((*state)[j][i] == 1){
				if(attacking_factor[j][i] > 0){
						if (attacking_factor[j][i] == INT_MAX) value += 2;
						else value -= safety_factor[j][i] - attacking_factor[j][i];
						if (p_id == 1) damage = min(damage, -1);
					}
			}
			if(free_space[j][i] > 0){
				value += 1;
			}
			if(free_space[j][i] < 0){
				value -= 1;
			}
		}
	}
	value += damage;

	if(my_id == 1) value = -1*value;

	return value;
}


matrix computeState(matrix* state, string neighbour){
	matrix new_state = (*state);
	int x1 = neighbour[2] - '0';
	int y1 = neighbour[4] - '0';
	int x2 = neighbour[8] - '0';
	int y2 = neighbour[10] - '0';
	if((*state)[y2][x2] == 2 || (*state)[y2][x2] ==4) check = 1;
	else check = 0;
	if(neighbour[6] == 'B'){
		new_state[y2][x2] = 0;
	}
	else{
		new_state[y2][x2] = (*state)[y1][x1];
		new_state[y1][x1] = 0;
	}
	return new_state;
}

vector<int> setRank(matrix* state, vector<string>* neighbour, char order = 'd'){
	int N = (*neighbour).size();
	vector<int> V(N);
	for(int i=0; i<N; i++){
		matrix tempState = computeState(state, (*neighbour)[i]);
		V[i] = evaluation_function(&tempState, my_id);
	}
	vector<int> ranking(N);
	int x=0;
	std::iota(ranking.begin(),ranking.end(),x++); //Initializing
	if(order == 'd' )	sort( ranking.begin(),ranking.end(), [&](int i,int j){return V[i]>V[j];} );
	else	sort( ranking.begin(),ranking.end(), [&](int i,int j){return V[i]<V[j];} );

	return ranking;
}


int minNode(matrix* state, int alpha, int beta, int depth);


int maxNode(matrix* state, int alpha, int beta, int depth){
	player_id = my_id;
	if(check == 1){
		int count0 = 0;
		int count1 = 0;
		for(int i=0 ; i<(*state).size(); i++){
			for(int j=0; j<(*state)[i].size(); j++){
				if((*state)[i][j] == 2) count1++;
				if((*state)[i][j] == 4) count0++;
			}
		}
		if(count0 <= 2 || count1 <= 2) {
			if(my_id == 0) return ((INT_MAX-2)/2)*(count0-count1);
			if(my_id == 1) return ((INT_MAX-2)/2)*(count1-count0);
		}
	}
	if(depth == 0){
		return evaluation_function(state, my_id);
	}
	vector<string> neighbour = getAllNeighbour(state);
	vector<int> ranking = setRank(state, &neighbour, 'd');

	int v = INT_MIN;
	int pos;
	for(int i=0; i<neighbour.size(); i++){
		matrix tempState = computeState(state, neighbour[ranking[i]]);
		int t = minNode(&tempState, alpha, beta, depth-1);
		if(t> v){
			pos = ranking[i];
		}
		v = max(v, t);
		alpha = max(alpha, v);
		if(alpha > beta){
			return alpha;
		}
	}
	mv = neighbour[pos];
	return v;
}

int minNode(matrix* state, int alpha, int beta, int depth){
	player_id = (my_id + 1)%2;
	if(check == 1){
		int count0 = 0;
		int count1 = 0;
		for(int i=0 ; i<(*state).size(); i++){
			for(int j=0; j<(*state)[i].size(); j++){
				if((*state)[i][j] == 2) count1++;
				if((*state)[i][j] == 4) count0++;
			}
		}
		if(count0 <= 2 || count1 <= 2) {
			if(my_id == 0) return ((INT_MAX-2)/2)*(count0-count1);
			if(my_id == 1) return ((INT_MAX-2)/2)*(count1-count0);
		}
	}
	if(depth == 0){
		return evaluation_function(state, my_id);
	}

	vector<string> neighbour = getAllNeighbour(state);
	vector<int> ranking = setRank(state, &neighbour, 'i');

	int v = INT_MAX;
	for(int i=0; i<neighbour.size(); i++){
		matrix tempState = computeState(state, neighbour[ranking[i]]);
		v = min(v, maxNode(&tempState, alpha, beta, depth-1));
		beta = min(beta, v);
		if(alpha > beta){
			return beta;
		}
	}
	return v;
}

int main(int argc, char const *argv[]){
	matrix state;
	std::vector<int> r1={2,1,2,1,2,1,2,1};
	std::vector<int> r2={0,1,0,1,0,1,0,1};
	std::vector<int> r3={0,1,0,1,0,1,0,1};
	std::vector<int> r4={0,0,0,0,0,0,0,0};
	std::vector<int> r5={0,0,0,0,0,0,0,0};
	std::vector<int> r6={3,0,3,0,3,0,3,0};
	std::vector<int> r7={3,0,3,0,3,0,3,0};
	std::vector<int> r8={3,4,3,4,3,4,3,4};
	state.push_back(r1);state.push_back(r2);state.push_back(r3);state.push_back(r4);
	state.push_back(r5);state.push_back(r6);state.push_back(r7);state.push_back(r8);

	string client;
	getline(cin,client);
	my_id = client[0] - '1';
	int opponentTurn;
	if(my_id == 0){
		opponentTurn = 0;
	}
	else{
		opponentTurn = 1;
	}
	player_id = 0;
	check = 0;

	while(true){
		if(opponentTurn == 1){
			mv = "";
			while(mv == ""){
				getline(cin, mv);
			}
			state = computeState(&state, mv);
		}
		int v = maxNode(&state, INT_MIN, INT_MAX, argv[1][0] - '0');
		state = computeState(&state, mv);
		cout<<mv<<endl;
		opponentTurn = 1;
		
	 }
}