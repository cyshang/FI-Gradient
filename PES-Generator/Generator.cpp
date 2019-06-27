#include "Generator.h"
#include "MyException.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

extern const string inDIR("input/");
extern const string outDIR("output/");
extern const string tmpDIR("tmp/");
extern const string templateDIR("template/");
extern const string indent = "    ";
extern string pes_name;
extern int dimP;
extern int dimR;
extern int dimX;

int nLayer(0);
int totalNode(0);
int totalWeight(0);
int totalBias(0);

vector<int> nNode;
vector<int> nWeight;
vector<int> nBias;
vector<string> minP;
vector<string> maxP;
string minE;
string maxE;
vector<vector<string>> weight;
vector<vector<string>> bias;

void readPesParameter()
{
	string inFile;
	string outFile;
	inFile = inDIR + "network.dat";
	ifstream fin(inFile.c_str(), ifstream::in);
	openCheck(fin, inFile);

	fin >> nLayer;
	nLayer += 2;

	nNode.resize(nLayer);
	nWeight.resize(nLayer, 0);
	nBias.resize(nLayer, 0);
	weight.resize(nLayer);
	bias.resize(nLayer);
	minP.resize(dimP);
	maxP.resize(dimP);

	for (int i = 0; i < nLayer - 1; ++i) {
		fin >> nNode[i];
	}
	nNode[nLayer - 1] = 1;

	if (nNode[0] != dimP) {
		throw string("Error: nNode[0] != dimP");
	}

	totalNode = 0;
	for (int i = 0; i < nLayer; ++i) {
		totalNode += nNode[i];
	}

	int totalWb;
	fin >> totalWb;
	fin.ignore(1024, '\n');

	for (int i = 1; i < nLayer; ++i) {
		nWeight[i] = nNode[i] * nNode[i - 1];
		nBias[i] = nNode[i];
	}

	//--------------- check ---------------
	{
		int check(0);

		for (int i = 1; i < nLayer; ++i) {
			check += nWeight[i];
			check += nNode[i];
		}

		if (check != totalWb) {
			throw string("Error: totalWeight check failed!");
		}
	}

	totalWeight = 0;
	for (int i = 1; i < nLayer; ++i) {
		totalWeight += nWeight[i];
	}

	totalBias = 0;
	for (int i = 1; i < nLayer; ++i) {
		totalBias += nBias[i];
	}

	for (int i = 0; i < dimP; ++i) {
		fin >> minP[i] >> maxP[i];
		fin.ignore(1024, '\n');
	}

	fin >> minE >> maxE;
	fin.ignore(1024, '\n');

	for (int iLayer = 1; iLayer < nLayer; ++iLayer) {
		for (int iNode = 0; iNode < nNode[iLayer]; ++iNode) {
			fin.ignore(1024, '\n');
		}
	}

	for (int iLayer = 1; iLayer < nLayer; ++iLayer) {
		weight[iLayer].resize(nWeight[iLayer]);
		bias[iLayer].resize(nBias[iLayer]);
	}

	for (int iLayer = 1; iLayer < nLayer; ++iLayer) {
		for (int row = 0; row < nNode[iLayer]; ++row) {
			for (int col = 0; col < nNode[iLayer - 1]; ++col) {
				int pos = col * nNode[iLayer] + row;
				fin >> weight[iLayer][pos];
				fin.ignore(1024, '\n');
			}
			fin >> bias[iLayer][row];
			fin.ignore(1024, '\n');
		}
	}
	fin.close();
}



void generatePesMod()
{
	string inFile;
	string outFile;
	inFile = templateDIR + "pes_mod.F90";
	outFile = tmpDIR + "pes_mod_" + pes_name + ".tmp";

	system((string("sed ") + R"('s/xxxxx/)" + pes_name + R"(/g' )" + inFile + " > " + outFile).c_str());

	ostringstream sout;

	//----- sed nLayer -----
	sout << nLayer - 1;
	system((string("sed -i ") + R"('s/PES_NL/)" + sout.str() + R"(/g' )" + outFile).c_str());
	sout.str("");

	//----- sed tNode -----
	sout << totalNode;
	system((string("sed -i ") + R"('s/PES_TNODE/)" + sout.str() + R"(/g' )" + outFile).c_str());
	sout.str("");

	//----- sed tW -----
	sout << totalWeight;
	system((string("sed -i ") + R"('s/PES_TW/)" + sout.str() + R"(/g' )" + outFile).c_str());
	sout.str("");

	//----- sed tb -----
	sout << totalBias;
	system((string("sed -i ") + R"('s/PES_TB/)" + sout.str() + R"(/g' )" + outFile).c_str());
	sout.str("");

	//----- sed dimP -----
	sout << dimP;
	system((string("sed -i ") + R"('s/PES_DIMP/)" + sout.str() + R"(/g' )" + outFile).c_str());
	sout.str("");

	//----- sed dimR -----
	sout << dimR;
	system((string("sed -i ") + R"('s/PES_DIMR/)" + sout.str() + R"(/g' )" + outFile).c_str());
	sout.str("");

	//----- sed dimX -----
	sout << dimX;
	system((string("sed -i ") + R"('s/PES_DIMX/)" + sout.str() + R"(/g' )" + outFile).c_str());
	sout.str("");

	//----- sed nNode -----
	sout << "(/";
	for (int i = 0; i < nLayer; ++i) {
		sout << nNode[i];
		if (i < nLayer - 1)
			sout << ", ";
	}
	sout << "/)";
	system((string("sed -i ") + R"('s;PES_NNODE;)" + sout.str() + R"(;g' )" + outFile).c_str());
	sout.str("");

	//----- sed nW -----
	sout << "(/";
	for (int i = 1; i < nLayer; ++i) {
		sout << nWeight[i];
		if (i < nLayer - 1)
			sout << ", ";
	}
	sout << "/)";
	system((string("sed -i ") + R"('s;PES_NW;)" + sout.str() + R"(;g' )" + outFile).c_str());
	sout.str("");

	inFile = outFile;
	outFile = outDIR + "pes_mod_" + pes_name + ".F90";
	ifstream fin(inFile.c_str(), ifstream::in);
	ofstream fout(outFile.c_str(), ofstream::out);

	string line;
	while (getline(fin, line)) {
		if (line != "!___insert_network_parameters") {
			fout << line << endl;
			continue;
		}

		int pos(0);
		int count(0);
		// --------------- data/weight/ ---------------
		fout << indent << "data W /" << "  &" << endl;
		for (int iL = 1; iL < nLayer; ++iL) {
			for (int iW = 0; iW < nWeight[iL]; ++iW) {
				if (pos == 0) {
					fout << indent << setw(23) << right << weight[iL][iW];
				}
				else {
					fout << setw(27) << right << weight[iL][iW];
				}

				if (++count < totalWeight) {
					fout << ',';
					if (pos == 2)
						fout << "  &" << endl;
				}
				else {
					fout << "   /" << endl;
				}
				pos = (pos + 1) % 3;
			}
		}
		fout << endl;

		// --------------- data/bias/ ---------------
		pos = 0;
		count = 0;
		fout << indent << "data b /  &" << endl;
		for (int iL = 1; iL < nLayer; ++iL) {
			for (int ib = 0; ib < nBias[iL]; ++ib) {
				if (pos == 0) {
					fout << indent << setw(23) << right << bias[iL][ib];
				}
				else {
					fout << setw(27) << right << bias[iL][ib];
				}

				if (++count < totalBias) {
					fout << ',';
					if (pos == 2)
						fout << "  &" << endl;
				}
				else {
					fout << "   /" << endl;
				}

				pos = (pos + 1) % 3;
			}
		}
		fout << endl;

		// --------------- data/minP/ ---------------
		pos = 0;
		fout << indent << "data minP /  &" << endl;
		for (int i = 0; i < dimP; ++i) {
			if (pos == 0) {
				fout << indent << setw(23) << right << minP[i];
			}
			else {
				fout << setw(27) << right << minP[i];
			}

			if (i < dimP - 1) {
				fout << ',';
				if (pos == 2) {
					fout << "  &" << endl;
				}
			}
			else {
				fout << "   /" << endl;
			}

			pos = (pos + 1) % 3;
		}
		fout << endl;

		// --------------- data/maxP/ ---------------
		pos = 0;
		fout << indent << "data maxP /  &" << endl;
		for (int i = 0; i < dimP; ++i) {
			if (pos == 0) {
				fout << indent << setw(23) << right << maxP[i];
			}
			else {
				fout << setw(27) << right << maxP[i];
			}

			if (i < dimP - 1) {
				fout << ',';
				if (pos == 2) {
					fout << "  &" << endl;
				}
			}
			else {
				fout << "   /" << endl;
			}

			pos = (pos + 1) % 3;
		}
		fout << endl;

		// --------------- data/minE, maxE/ ---------------
		fout << indent << "data minE /" << minE << '/' << endl;
		fout << endl;
		fout << indent << "data maxE /" << maxE << '/' << endl;
		fout << endl;
	}
	fin.close();
	fout.close();
}
