#include <bits/stdc++.h>
using namespace std;

typedef vector<vector<int>> matrix;
template <typename T>
std::vector<T> operator+(const std::vector<T>& a, const std::vector<T>& b){
    assert(a.size() == b.size());

    std::vector<T> result;
    result.reserve(a.size());

    std::transform(a.begin(), a.end(), b.begin(), 
                   std::back_inserter(result), std::plus<T>());
    return result;
}
template <typename T>
std::vector<T> operator-(const std::vector<T>& a, const std::vector<T>& b){
    assert(a.size() == b.size());

    std::vector<T> result;
    result.reserve(a.size());

    std::transform(a.begin(), a.end(), b.begin(), 
                   std::back_inserter(result), std::minus<T>());
    return result;
}

void print_intvector(std::vector<int> v){
	for (std::vector<int>::iterator i = v.begin(); i != v.end(); ++i){
		std::cout << *i << ' ' ;
	}
	std::cout << std::endl;
}
void print_boardrow(std::vector<int> v){
	cout << '|';
	for (std::vector<int>::iterator i = v.begin(); i != v.end(); ++i){
		if(*i == 0){
			std::cout <<  " |" ;
		}
		else{
			std::cout << *i << '|' ;
		}
		
	}
	std::cout << std::endl;
}
void print_matrix(matrix mat){
	for (matrix::iterator i = mat.begin(); i != mat.end(); ++i){
		print_intvector(*i);
	}
	cout << "---------------" << endl;
}
void print_board(matrix mat){
	for (matrix::iterator i = mat.begin(); i != mat.end(); ++i){
		print_boardrow(*i);
	}
	cout << "-----------------" << endl;
}


bool valid_pos(vector<int> *pos, int n = 8, int m = 8){
	if((*pos)[0] >=0 and (*pos)[1] >=0 and (*pos)[0] <=(n-1) and (*pos)[1] <=(m-1)){
		return true;
	}
	return false;
}

void change_state(string move, matrix* state){
	int x1 = move[2] - '0';
	int y1 = move[4] - '0';

	if((*state)[y1][x1] != 1 && (*state)[y1][x1] != 3 ){
		cout << "Soldier Not here!" << endl;
		return;
	}
	char t = move[6];
	int x2 = move[8]- '0';
	int y2 = move[10]- '0';
	if(t == 'B'){
		(*state)[y2][x2] = 0;
	}
	else if(t == 'M'){
		(*state)[y2][x2] = (*state)[y1][x1];
		(*state)[y1][x1] = 0;
	}
}

matrix forward_steps(vector<int> pos){
	matrix ret;
	ret.reserve(3);
	std::vector<int> v;
	std::vector<int> t;
	if (player_id == 1){
		t = {0,1};
		v = pos + t;
		if(valid_pos(&v)){
			ret.push_back(v);
		}
		t = {-1,1};
		v = pos + t;
		if(valid_pos(&v)){
			ret.push_back(v);
		}
		t = {1,1};
		v = pos + t;
		if(valid_pos(&v)){
			ret.push_back(v);
		}
	}
	else if(player_id == 0){
		t = {0,-1};
		v = pos + t;
		if(valid_pos(&v)){
			ret.push_back(v);
		}
		t = {-1,-1};
		v = pos + t;
		if(valid_pos(&v)){
			ret.push_back(v);
		}
		t = {1,-1};
		v = pos + t;
		if(valid_pos(&v)){
			ret.push_back(v);
		}
	}
	return ret;
}

matrix retreat_steps(vector<int> pos){

	matrix ret;
	ret.reserve(3);
	std::vector<int> v;
	std::vector<int> t;
	if (player_id == 1){
		t = {0,-2};
		v = pos + t;
		if(valid_pos(&v)){
			ret.push_back(v);
		}
		t = {-2,-2};
		v = pos + t;
		if(valid_pos(&v)){
			ret.push_back(v);
		}
		t = {2,-2};
		v = pos + t;
		if(valid_pos(&v)){
			ret.push_back(v);
		}
	}
	else if(player_id == 0){
		t = {0,2};
		v = pos + t;
		if(valid_pos(&v)){
			ret.push_back(v);
		}
		t = {-2,2};
		v = pos + t;
		if(valid_pos(&v)){
			ret.push_back(v);
		}
		t = {2,2};
		v = pos + t;
		if(valid_pos(&v)){
			ret.push_back(v);
		}
	}
	return ret;
}

matrix capture_side(vector<int> pos){
	matrix ret;
	ret.reserve(2);
	std::vector<int> v;
	std::vector<int> t;
	t = {1,0};
	v = pos + t;
	if(valid_pos(&v)){
		ret.push_back(v);
	}
	t = {-1,0};
	v = pos + t;
	if(valid_pos(&v)){
		ret.push_back(v);
	}
	return ret;
}

matrix find_soldiers(matrix* state, int player_id = player_id){
	matrix ret;
	ret.reserve(12);
	if (player_id == 0){
		for (int i = 0; i < (*state).size(); ++i){
			for (int j = 0; j < (*state)[0].size(); ++j){
				if((*state)[i][j] == 3){
					ret.push_back({j,i});	
				}	
			}
		}
	}
	else if (player_id == 1){
		for (int i = 0; i < (*state).size(); ++i){
			for (int j = 0; j < (*state)[0].size(); ++j){
				if((*state)[i][j] == 1){
					ret.push_back({j,i});	
				}
			}
		}
	}
	return ret;
}

bool is_enemy_adjacent(matrix* state, vector<int> pos){
	if(player_id == 0){
		if((*state)[pos[1]][pos[0]] != 3){
			cout << "Soldier not at position!" << endl;
			return false;
		}
		std::vector<int> v;
		std::vector<int> t;
		t = {0,-1};
		v = pos + t;
		if(valid_pos(&v) && ((*state)[v[1]][v[0]] == 1)){
			return true;
		}
		t = {-1,-1};
		v = pos + t;
		if(valid_pos(&v) && ((*state)[v[1]][v[0]] == 1)){
			return true;
		}
		t = {1,-1};
		v = pos + t;
		if(valid_pos(&v) && ((*state)[v[1]][v[0]] == 1)){
			return true;
		}
		t = {1,0};
		v = pos + t;
		if(valid_pos(&v) && ((*state)[v[1]][v[0]] == 1)){
			return true;
		}
		t = {-1,0};
		v = pos + t;
		if(valid_pos(&v) && ((*state)[v[1]][v[0]] == 1)){
			return true;
		}
	}
	else if (player_id == 1){
		if((*state)[pos[1]][pos[0]] != 1){
			cout << "Soldier not at position!" << endl;
			return false;
		}
		std::vector<int> v;
		std::vector<int> t;
		t = {0,1};
		v = pos + t;
		if(valid_pos(&v) && ((*state)[v[1]][v[0]] == 3)){
			return true;
		}
		t = {-1,1};
		v = pos + t;
		if(valid_pos(&v) && ((*state)[v[1]][v[0]] == 3)){
			return true;
		}
		t = {1,1};
		v = pos + t;
		if(valid_pos(&v) && ((*state)[v[1]][v[0]] == 3)){
			return true;
		}
		t = {1,0};
		v = pos + t;
		if(valid_pos(&v) && ((*state)[v[1]][v[0]] == 3)){
			return true;
		}
		t = {-1,0};
		v = pos + t;
		if(valid_pos(&v) && ((*state)[v[1]][v[0]] == 3)){
			return true;
		}
	}
	return false;
}

vector<int> soldier_possible_moves(matrix* state, vector<int> pos){

	vector<int> moves;
	moves.reserve(12);
	if(player_id == 1){
		if((*state)[pos[1]][pos[0]] != 1){
			cout << "Soldier not at position!" << endl;
			return moves;
		}
		matrix forw_st = forward_steps(pos);
		for (int i = 0; i < forw_st.size(); ++i){
			if((*state)[forw_st[i][1]][forw_st[i][0]] == 0 || (*state)[forw_st[i][1]][forw_st[i][0]] == 3 || (*state)[forw_st[i][1]][forw_st[i][0]] == 4){
				moves.push_back(forw_st[i][0]);
				moves.push_back(forw_st[i][1]);
			}
		}
		
		if(is_enemy_adjacent(state, pos)){
			matrix retr_st = retreat_steps(pos);
			for (int i = 0; i < retr_st.size(); ++i){
				if((*state)[retr_st[i][1]][retr_st[i][0]] == 0 || (*state)[retr_st[i][1]][retr_st[i][0]] == 3 || (*state)[retr_st[i][1]][retr_st[i][0]] == 4){
					moves.push_back(retr_st[i][0]);
					moves.push_back(retr_st[i][1]);
				}
			}
		}
		
		
		matrix capt_sd = capture_side(pos);
		for (int i = 0; i < capt_sd.size(); ++i){
			if((*state)[capt_sd[i][1]][capt_sd[i][0]] == 3 || (*state)[capt_sd[i][1]][capt_sd[i][0]] == 4){
				moves.push_back(capt_sd[i][0]);
				moves.push_back(capt_sd[i][1]);
			}
		}
	}

	else if(player_id == 0){
		if((*state)[pos[1]][pos[0]] != 3){
			cout << "Soldier not at position!" << endl;
			return moves;
		}
		
		matrix forw_st = forward_steps(pos);
		for (int i = 0; i < forw_st.size(); ++i){
			if((*state)[forw_st[i][1]][forw_st[i][0]] == 0 || (*state)[forw_st[i][1]][forw_st[i][0]] == 1 || (*state)[forw_st[i][1]][forw_st[i][0]] == 2){
				moves.push_back(forw_st[i][0]);
				moves.push_back(forw_st[i][1]);
			}
		}
		
		if(is_enemy_adjacent(state, pos)){
			matrix retr_st = retreat_steps(pos);
			for (int i = 0; i < retr_st.size(); ++i){
				if((*state)[retr_st[i][1]][retr_st[i][0]] == 0 || (*state)[retr_st[i][1]][retr_st[i][0]] == 1 || (*state)[retr_st[i][1]][retr_st[i][0]] == 2){
					moves.push_back(retr_st[i][0]);
					moves.push_back(retr_st[i][1]);
				}
			}
		}

		matrix capt_sd = capture_side(pos);
		for (int i = 0; i < capt_sd.size(); ++i){
			if((*state)[capt_sd[i][1]][capt_sd[i][0]] == 1 || (*state)[capt_sd[i][1]][capt_sd[i][0]] == 2){
				moves.push_back(capt_sd[i][0]);
				moves.push_back(capt_sd[i][1]);
			}
		}
		
	}

	return moves;
}

matrix find_cont_soldiers(vector<int> pos, string dir, matrix* sold_pos){

	vector<int> ver_down = { 0, 1};
	vector<int> ver_up   = { 0,-1};
	vector<int> hor_rigt = { 1, 0};
	vector<int> hor_left = {-1, 0};
	vector<int> top_rigt = { 1,-1};
	vector<int> top_left = {-1,-1};
	vector<int> bot_rigt = { 1, 1};
	vector<int> bot_left = {-1, 1}; 
	vector<int> v = pos;
	matrix ret;
	ret.reserve(4);
	
	if(dir == "vertical"){
		while(true){
			v = v + ver_up;
			if(find((*sold_pos).begin(), (*sold_pos).end(), v) != (*sold_pos).end()){
				ret.insert(ret.begin(),v);continue;
			}
			else{break;}
		}
		v = pos;
		ret.push_back(v);
		while(true){
			v = v + ver_down;
			if(find((*sold_pos).begin(), (*sold_pos).end(), v) != (*sold_pos).end()){
				ret.push_back(v);continue;
			}
			else{break;}
		}
	}
	else if(dir == "horizontal"){
		while(true){
			v = v + hor_rigt;
			if(find((*sold_pos).begin(), (*sold_pos).end(), v) != (*sold_pos).end()){
				ret.insert(ret.begin(),v);continue;
			}
			else{break;}
		}
		v = pos;
		ret.push_back(v);
		while(true){
			v = v + hor_left;
			if(find((*sold_pos).begin(), (*sold_pos).end(), v) != (*sold_pos).end()){
				ret.push_back(v);continue;
			}
			else{break;}
		}
	}

	else if(dir == "diagonal_origin"){
		while(true){
			v = v + top_left;
			if(find((*sold_pos).begin(), (*sold_pos).end(), v) != (*sold_pos).end()){
				ret.insert(ret.begin(),v);continue;
			}
			else{break;}
		}
		v = pos;
		ret.push_back(v);
		while(true){
			v = v + bot_rigt;
			if(find((*sold_pos).begin(), (*sold_pos).end(), v) != (*sold_pos).end()){
				ret.push_back(v);continue;
			}
			else{break;}
		}
	}
	else if(dir == "diagonal_non_origin"){
		while(true){
			v = v + top_rigt;
			if(find((*sold_pos).begin(), (*sold_pos).end(), v) != (*sold_pos).end()){
				ret.insert(ret.begin(),v);continue;
			}
			else{break;}
		}
		v = pos;
		ret.push_back(v);
		while(true){
			v = v + bot_left;
			if(find((*sold_pos).begin(), (*sold_pos).end(), v) != (*sold_pos).end()){
				ret.push_back(v);continue;
			}
			else{break;}
		}
	}
	return ret;
}

vector<matrix> find_cannons(matrix sold_pos){
	vector<matrix> ret;
	matrix temp1,temp2,temp3, temp4;
	for (int i = 0; i < sold_pos.size(); ++i)	{

		temp1 = find_cont_soldiers(sold_pos[i], "vertical", 			&sold_pos);
		temp2 = find_cont_soldiers(sold_pos[i], "horizontal", 			&sold_pos);
		temp3 = find_cont_soldiers(sold_pos[i], "diagonal_origin", 		&sold_pos);
		temp4 = find_cont_soldiers(sold_pos[i], "diagonal_non_origin", 	&sold_pos);

		if(temp1.size() >= 3){
			if(std::find(ret.begin(), ret.end(), temp1) != ret.end()) {
    			continue;
			} 
			else {
    			ret.push_back(temp1);
			}
			
		}
		if(temp2.size() >= 3){
			if(std::find(ret.begin(), ret.end(), temp2) != ret.end()) {
    			continue;
			} 
			else {
    			ret.push_back(temp2);
			}
		}
		if(temp3.size() >= 3){
			if(std::find(ret.begin(), ret.end(), temp3) != ret.end()) {
    			continue;
			} 
			else {
    			ret.push_back(temp3);
			}
		}
		if(temp4.size() >= 3){
			if(std::find(ret.begin(), ret.end(), temp4) != ret.end()) {
    			continue;
			} 
			else {
    			ret.push_back(temp4);
			}
		}
	}

	return ret;
}

vector<string> bomb_cannon(matrix* state, matrix cannon){

	vector<string> ret;
	int k = cannon.size();
	vector<int> dif1 = cannon[0]   - cannon[1];
	vector<int> dif2 = cannon[k-1] - cannon[k-2];

	if(player_id == 1){
		vector<int> t1   = cannon[0] + dif1;
		if(valid_pos(&t1) && ((*state)[t1[1]][t1[0]] ==  0)){
			t1 = t1 + dif1;
			if(valid_pos(&t1) && ((*state)[t1[1]][t1[0]] != 1) && ((*state)[t1[1]][t1[0]] !=  2)){
				ret.push_back("S " + to_string(cannon[0][0]) + " " + to_string(cannon[0][1]) + " B " + to_string(t1[0]) + " " + to_string(t1[1]));
			}
			t1 = t1 + dif1;
			if(valid_pos(&t1) && ((*state)[t1[1]][t1[0]] != 1) && ((*state)[t1[1]][t1[0]] !=  2)){
				ret.push_back("S " + to_string(cannon[0][0]) + " " + to_string(cannon[0][1]) + " B " + to_string(t1[0]) + " " + to_string(t1[1]));
			}
		}

		t1 = cannon[k-1] + dif2;
		if(valid_pos(&t1) && ((*state)[t1[1]][t1[0]] ==  0)){
			t1 = t1 + dif2;
			if(valid_pos(&t1) && ((*state)[t1[1]][t1[0]] != 1) && ((*state)[t1[1]][t1[0]] !=  2)){
				ret.push_back("S " + to_string(cannon[k-1][0]) + " " + to_string(cannon[k-1][1]) + " B " + to_string(t1[0]) + " " + to_string(t1[1]));
			}
			t1 = t1 + dif2;
			if(valid_pos(&t1) && ((*state)[t1[1]][t1[0]] != 1) && ((*state)[t1[1]][t1[0]] !=  2)){
				ret.push_back("S " + to_string(cannon[k-1][0]) + " " + to_string(cannon[k-1][1]) + " B " + to_string(t1[0]) + " " + to_string(t1[1]));
			}
		}
	}

	else if(player_id == 0){
		vector<int> t1   = cannon[0] + dif1;
		if(valid_pos(&t1) && ((*state)[t1[1]][t1[0]] ==  0)){
			t1 = t1 + dif1;
			if(valid_pos(&t1) && ((*state)[t1[1]][t1[0]] != 3) && ((*state)[t1[1]][t1[0]] !=  4)){
				ret.push_back("S " + to_string(cannon[0][0]) + " " + to_string(cannon[0][1]) + " B " + to_string(t1[0]) + " " + to_string(t1[1]));
			}
			t1 = t1 + dif1;
			if(valid_pos(&t1) && ((*state)[t1[1]][t1[0]] != 3) && ((*state)[t1[1]][t1[0]] !=  4)){
				ret.push_back("S " + to_string(cannon[0][0]) + " " + to_string(cannon[0][1]) + " B " + to_string(t1[0]) + " " + to_string(t1[1]));
			}
		}

		t1 = cannon[k-1] + dif2;
		if(valid_pos(&t1) && ((*state)[t1[1]][t1[0]] ==  0)){
			t1 = t1 + dif2;
			if(valid_pos(&t1) && ((*state)[t1[1]][t1[0]] != 3) && ((*state)[t1[1]][t1[0]] !=  4)){
				ret.push_back("S " + to_string(cannon[k-1][0]) + " " + to_string(cannon[k-1][1]) + " B " + to_string(t1[0]) + " " + to_string(t1[1]));
			}
			t1 = t1 + dif2;
			if(valid_pos(&t1) && ((*state)[t1[1]][t1[0]] != 3) && ((*state)[t1[1]][t1[0]] !=  4)){
				ret.push_back("S " + to_string(cannon[k-1][0]) + " " + to_string(cannon[k-1][1]) + " B " + to_string(t1[0]) + " " + to_string(t1[1]));
			}
		}
	}
	return ret;
} 

vector<string> move_cannon(matrix* state, matrix cannon){

	vector<string> ret;
	int k = cannon.size();
	vector<int> dif1 = cannon[0]   - cannon[1];
	vector<int> dif2 = cannon[k-1] - cannon[k-2];
		vector<int> t1 = cannon[0] + dif1;
		if(valid_pos(&t1) && ((*state)[t1[1]][t1[0]] ==  0)){
			ret.push_back("S " + to_string(cannon[2][0]) + " " + to_string(cannon[2][1]) + " M " + to_string(t1[0]) + " " + to_string(t1[1]));
		}
		t1 = cannon[k-1] + dif2;
		if(valid_pos(&t1) && ((*state)[t1[1]][t1[0]] ==  0)){
			ret.push_back("S " + to_string(cannon[k-3][0]) + " " + to_string(cannon[k-3][1]) + " M " + to_string(t1[0]) + " " + to_string(t1[1]));
		}
	return ret;
} 
