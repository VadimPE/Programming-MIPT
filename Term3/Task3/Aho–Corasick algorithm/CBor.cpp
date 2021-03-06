//
//  CBor.cpp
//  Aho–Corasick algorithm
//
//  Created by Вадик  on 06.12.16.
//  Copyright © 2016 Вадик . All rights reserved.
//

#include "CBor.hpp"

CBor::CBor()
{
    CBor::CNode root_;
    nodes.push_back( root_ );
}

CBor::CNode::CNode():
    Son( 26, -1 ), Go( 26, -1 )
{
    SuffLink = -1;
    Up = -1;
    IsTerminal = false;
}

int CBor::GetSuffLink( int number )
{
    if( nodes[number].SuffLink == -1 ) {
        if( number == 0 || nodes[number].Parent == 0 ) {
            nodes[number].SuffLink = 0;
        } else {
            nodes[number].SuffLink = GetLink( GetSuffLink( nodes[number].Parent ), nodes[number].CharToParent );
        }
    }
    return nodes[number].SuffLink;
}

int CBor::GetLink( int number, char symbol )
{
    if( nodes[number].Go[symbol] == -1 ) {
        if( nodes[number].Son[symbol] != -1 ) {
            nodes[number].Go[symbol] = nodes[number].Son[symbol];
        } else {
            if( number == 0 ) {
                nodes[number].Go[symbol] = 0;
            } else {
                nodes[number].Go[symbol] = GetLink( GetSuffLink( number ), symbol );
            }
        }
    }
    return nodes[number].Go[symbol];
}

int CBor::GetUp( int number )
{
    if( nodes[number].Up == -1 ) {
        if( nodes[GetSuffLink( number )].IsTerminal ) {
            nodes[number].Up = GetSuffLink( number );
        } else {
            if( GetSuffLink( number ) == 0 ) {
                nodes[number].Up = 0;
            } else {
                nodes[number].Up = GetUp( GetSuffLink( number ) );
            }
        }
    }
    return nodes[number].Up;
}

void CBor::AddString( std::string str, int number )
{
    int cur = 0;
    for( int i = 0; i < str.length(); ++i ) {
        char symbol = str[i] - 'a';
        if( nodes[cur].Son[symbol] == -1 ) {
            CBor::CNode node;
            node.Parent = cur;
            node.CharToParent = symbol;
            nodes.push_back( node );
            nodes[cur].Son[symbol] = nodes.size() - 1;
        }
        cur = nodes[cur].Son[symbol];
    }
    nodes[cur].IsTerminal = true;
    nodes[cur].StrTerminal.push_back( number );
    length.push_back( str.length() );
}

void CBor::Check( int number, int pos, std::vector<int>& posString, std::vector<int>& posArray )
{
    for( int i = number; i != 0; i = GetUp( i ) ) {
        if( nodes[i].IsTerminal ) {
            for( auto j : nodes[i].StrTerminal ) {
                if( pos - length[j] + 1 - posArray[j] >= 0 ) {
                    ++posString[pos - length[j] + 1 - posArray[j]];
                }
            }
        }
    }
}

void CBor::ProcessText( std::string str, std::vector<int>& posString, std::vector<int>& pos )
{
    int node = 0;
    for( int i = 0; i < str.length(); ++i ) {
        char symbol = str[i] - 'a';
        node = GetLink( node, symbol );
        Check( node, i, posString, pos );
    }
}