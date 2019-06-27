#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

int main(int argc, char **argv)
{
	if (argc < 2) {
		cerr << "Error: missing file oprand!" << endl;
		exit(127);
	}

	string   argStr = argv[1];
	
	//=======================================================
	//==================== main part ========================
	
	ifstream fin(argStr.c_str(), ifstream::in);

	int nLayer;
	vector<int> nNode;

	fin >> nLayer;
	nLayer += 2;

	nNode.resize(nLayer);
	for (int i = 0; i < nLayer - 1; ++i) {
		fin >> nNode[i];
	}
	nNode[nLayer - 1] = 1;

	int totalWeight;
	fin >> totalWeight;

	fin.ignore(1024, '\n');

	vector<int> nWeight(nLayer, 0);
	vector<int> nBias(nLayer, 0);

	for (int i = 1; i < nLayer; ++i) {
		nWeight[i] = nNode[i] * nNode[i - 1];
		nBias[i] = nNode[i];
	}

	// check
	{
		int check(0);

		for (int i = 1; i < nLayer; ++i) {
			check += nWeight[i];
			check += nNode[i];
		}

		if (check != totalWeight) {
			cerr << "Error: totalWeight check failed!" << endl;
			exit(127);
		}
	}

	vector<string> minmaxX(nNode[0]);
	string minmaxE;

	for (int i = 0; i < nNode[0]; ++i) {
		getline(fin, minmaxX[i]);
	}
	getline(fin, minmaxE);

	for (int iLayer = 1; iLayer < nLayer; ++iLayer) {
		for (int iNode = 0; iNode < nNode[iLayer]; ++iNode) {
			fin.ignore(1024, '\n');
		}
	}

	vector<vector<string>> weight(nLayer);
	vector<vector<string>> bias(nLayer);

	for (int iLayer = 1; iLayer < nLayer; ++iLayer) {
		weight[iLayer].resize(nWeight[iLayer]);
		bias[iLayer].resize(nBias[iLayer]);
	}

	for (int iLayer = 1; iLayer < nLayer; ++iLayer) {
		for (int row = 0; row < nNode[iLayer]; ++row) {
			for (int col = 0; col < nNode[iLayer - 1]; ++col) {
				int pos = col * nNode[iLayer] + row;
				getline(fin, weight[iLayer][pos]);
			}
			getline(fin, bias[iLayer][row]);
		}
	}

	fin.close();
	//=======================================================

	ofstream fout("pes-net.out", ofstream::out);
		
	fout << "  " << nLayer - 2 << endl;
	
	for (int i = 0; i < nLayer - 1; ++i) {
		fout << "  " << nNode[i] << endl;
	}

	fout << "  " << totalWeight << endl;
	
	for (size_t i = 0; i < minmaxX.size(); ++i) {
		fout << minmaxX[i] << endl;
	}

	fout << minmaxE << endl;

	for (int iLayer = 1; iLayer < nLayer; ++iLayer) {
		for (size_t i = 0; i < weight[iLayer].size(); ++i) {
			fout << weight[iLayer][i] << endl;
		}

		for (size_t i = 0; i < bias[iLayer].size(); ++i) {
			fout << bias[iLayer][i] << endl;
		}
	}

	fout.close();
}