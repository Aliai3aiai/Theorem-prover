//
//  main.cpp
//  proover
//
//  Created by aichung on 2019/5/20.
//  Copyright © 2019年 aichung. All rights reserved.
//

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>

using namespace std;



class Clause {
public:
    vector<int> clause;
    int traceBack[4];
    
    Clause() { };
    Clause( vector<int> a, int* b ) {
        clause = a;
        for( int i=0; i<4; i++ ) traceBack[i] = b[i];
    }
    
    string clauseStr () {
        char temp;
        string temp2;
        if( clause.empty() ) temp2 = "Nil";
        else {
            for( int i=0; i<clause.size()-1; i++ ) {
                if( clause[i]>0) {
                    temp = clause[i];
                    temp2.push_back( temp ); temp2.push_back( '|' );
                } else {
                    temp = 0 - clause[i];
                    temp2.push_back( '!' ); temp2.push_back( temp ); temp2.push_back( '|' );
                }
            }
            if( clause.back() > 0 ) {
                temp = clause.back();
                temp2.push_back( temp );
            } else {
                temp = 0 - clause.back();
                temp2.push_back( '!' ); temp2.push_back( temp );
            }
        }
        return temp2;
    }
};


//main function
int main ()
{
    vector< Clause > S;
    vector<string> clauses;
    
    ifstream file;
    file.open ("sample.txt");
    if ( !file ) {
        cout << "Can not open file!" << endl;
        return 1;
    }
    char c;
    string str_temp;
    while( (c=file.get())!='0' ) {
        if ( c!=' ' ) str_temp.push_back(c);
        else {
            clauses.push_back( str_temp );
            str_temp.clear();
        }
    }
    file.close();
    
    for( int i=0; i<clauses.size(); i++ ) {
        vector<int> temp;
        int temp2;
        int flag = 0;
        for( int j=0; j<clauses[i].size(); j++) {
            if( clauses[i][j] != '|' ) {
                if( clauses[i][j]!='!' ) {
                    if( flag==0 ) temp2 = 0 + clauses[i][j];
                    else {
                        temp2 = 0 - clauses[i][j];
                        flag = 0;
                    }
                    temp.push_back( temp2 );
                } else flag = 1;
            }
        }
        int temp3[4] = { -1, -1, -1, -1 };
        S.push_back( Clause(temp, temp3) );
    }
    
    
    cout << "The original clause set is: " << endl;
    for( int i=0; i<clauses.size(); i++ ) {
        cout << clauses[i] << "  ";
    }
    cout << endl << endl;
    
    vector< vector<Clause> > R;
    R.push_back( S );
    Clause Nil;
    
    bool breadthFirst( vector< vector<Clause> > &R, Clause &Nil );
    int print_result ( vector< vector<Clause> > &R, Clause &Nil );
    if( breadthFirst( R, Nil ) ) {
        print_result ( R, Nil );
    }
    else cout << "The clause set is not unsastifiable. " << endl;
    
    
    c = getchar();
    return 0;
}



int print_result ( vector< vector<Clause> > &R, Clause &Nil )
{
    cout << "The resolution refutation tree is: " << endl << endl;
    vector< vector < vector<int> > > result;
    vector< vector<int> > temp;
    vector<int> temp2;
    
    temp2.push_back( 0 ); temp.push_back( temp2 ); result.push_back( temp );
    temp.clear();
    temp2.clear();
    temp2.push_back( 0 ); temp2.push_back( Nil.traceBack[0] ); temp2.push_back( Nil.traceBack[1] );
    temp.push_back( temp2 );
    temp2.clear();
    temp2.push_back( 0 ); temp2.push_back( Nil.traceBack[2] ); temp2.push_back( Nil.traceBack[3] );
    temp.push_back( temp2 );
    result.push_back( temp );
    
    int n;
    if( Nil.traceBack[0]>Nil.traceBack[2] ) n = Nil.traceBack[0];
    else n = Nil.traceBack[2];
    int foobar = 1;
    for( int i=0; i<n+1; i++ ) foobar = foobar*2;
    
    for( int i=2; i<n+2; i++ ) {
        temp.clear();
        int foo = 1; for( int bar=0; bar<i; bar++ ) foo = foo * 2;
        for( int j=0; j < foo; j = j+2 ) {
            
            if( (result[i-1][j/2][0] == 0) && (R[ result[i-1][j/2][1] ][ result[i-1][j/2][2] ].traceBack[0] != -1) ) {
                temp2.clear();
                temp2.push_back( 0 );
                temp2.push_back( R[ result[i-1][j/2][1] ][ result[i-1][j/2][2] ].traceBack[0] );
                temp2.push_back( R[ result[i-1][j/2][1] ][ result[i-1][j/2][2] ].traceBack[1] );
                temp.push_back( temp2 );
                temp2.clear();
                temp2.push_back( 0 );
                temp2.push_back( R[ result[i-1][j/2][1] ][ result[i-1][j/2][2] ].traceBack[2] );
                temp2.push_back( R[ result[i-1][j/2][1] ][ result[i-1][j/2][2] ].traceBack[3] );
                temp.push_back( temp2 );
            } else {
                temp2.clear();
                temp2.push_back( 1 );
                temp.push_back( temp2 );
                temp2.clear();
                temp2.push_back( 1 );
                temp.push_back( temp2 );
            }
        }
        result.push_back( temp );
    }
    
    float flag = 0;
    for( int i=n+1; i > 0; i-- ) {
        int foo = 1; for( int bar=0; bar<i; bar++ ) foo = foo * 2;
        int space = foobar - foo;
        
        if( flag != 0 ) cout << "    ";
        for( int j=0; j<flag-0.5; j++ ) cout << "        ";
        for( int j=0; j<foo; j++ ) {
            if( result[i][j][0] == 0 ) {
                char * cstr;
                string str = R[ result[i][j][1] ][ result[i][j][2] ].clauseStr();
                cstr = new char [str.size()+1];
                strcpy (cstr, str.c_str());
                printf( "%-8s",  cstr );
                delete[] cstr;
            } else cout << "        ";
            for( int k=0; k<flag*2; k++ ) cout << "        ";
        }
        cout << endl;
        
        
        if( flag != 0 ) cout << "    ";
        for( int j=0; j<flag-0.5; j++ ) cout << "        ";
        for( int j=0; j<foo; j=j+2 ) {
            if( result[i][j][0] == 0 ) cout << "  \\     ";
            else cout << "        ";
            for( int k=0; k<flag*2; k++ ) cout << "        ";
            if( result[i][j][0] == 0 ) cout << "/       ";
            else cout << "        ";
            for( int k=0; k<flag*2; k++ ) cout << "        ";
        }
        cout << endl;
        
        flag = flag*2+0.5;
    }
    
    cout << "    ";
    for( int j=0; j<flag-0.5; j++ ) cout << "        ";
    cout << "Nil" << endl << endl;
    return 0;
}



bool breadthFirst( vector< vector<Clause> > &R, Clause &Nil )
{
    bool resolution ( Clause &a, Clause &b, Clause &c, int &i, int &j, int &l, int &k, vector< vector<Clause> > &R );
    for( int i=0; i<R.size(); i++ ) {
        vector<Clause> tempS;
        for( int l=0; l<R[i].size(); l++ ) {
            for( int j=0; j<R.size(); j++ ) {
                for( int k=0; k<R[j].size(); k++ ) {
                    Clause tempC;
                    int temp = resolution( R[i][l], R[j][k], tempC, i, j, l, k, R );
                    if( temp==1 ) {
                        tempS.push_back( tempC );
                    }
                    else if( temp==0 ) {
                        Nil.clause = tempC.clause;
                        for( int foo=0; foo<4; foo++ ) Nil.traceBack[foo] = tempC.traceBack[foo];
                        return true;
                    }
                }
            }
        }
        if( !tempS.empty() ) R.push_back( tempS );
    }
    return false;
}


int resolution ( Clause &a, Clause &b, Clause &c, int &i, int &j, int &l, int &k, vector< vector<Clause> > &R )
{
    bool clauseCompare( vector<int>, vector<int> );
    for( int x=0; x<a.clause.size(); x++ ) {
        for( int y=0; y<b.clause.size(); y++ ) {
            if( a.clause[x] + b.clause[y] == 0 ) {
                vector<int> temp;
                for( int foo=0; foo<a.clause.size(); foo++ ) {
                    if( foo!=x ) temp.push_back( a.clause[foo] );
                }
                for( int foo=0; foo<b.clause.size(); foo++) {
                    if( foo!=y ) {
                        int foobar = 0;
                        for( int bar=0; bar<x; bar++ ) if( a.clause[bar]==b.clause[foo] ) foobar++;
                        for( int bar=x+1; bar<a.clause.size(); bar++ ) if( a.clause[bar]==b.clause[foo] ) foobar++;
                        if( foobar==0 ) temp.push_back( b.clause[foo] );
                    }
                }
                c.clause = temp;
                c.traceBack[0] = i; c.traceBack[1] = l; c.traceBack[2] = j; c.traceBack[3] = k;
                
                if( temp.empty() ) return 0;
                
                int flag = 0;
                for( int u=0; u<R.size(); u++ ) {
                    for( int v=0; v<R[u].size(); v++ ) {
                        if( clauseCompare( temp, R[u][v].clause ) ) {
                            flag++;
                            break;
                        }
                    }
                }
                if( flag==0 ) return 1;
                else return 2;
            }
        }
    }
    return 2;
}



bool clauseCompare ( vector<int> a, vector<int> b )
{
    if( a.size() != b.size() ) return false;
    sort( a.begin(), a.end() );
    sort( b.begin(), b.end() );
    for( int i=0; i<a.size(); i++ ) {
        if( a[i] != b[i] ) return false;
    }
    return true;
}


