/**
 * cluster_collection.cpp Copyright 2010
 * Dr. Homayoun Valafar, Zach Swearingen
 *
 * This file is part of Redcraft.
 *
 * Redcraft is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Redcraft is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Redcraft.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "cluster_collection.h"
#include "tstorage.h"
#include "polypeptide.h"

#include <cmath>

inline bool dcomp(double &d1, double &d2){
	return abs(d1 - d2) < 0.0000001;
}

bool operator<(ClusterCoordinates v1, ClusterCoordinates v2){
	if(v1.cX < v2.cX) return true; if(v1.cX > v2.cX) return false;
	if(v1.cY < v2.cY) return true; if(v1.cY > v2.cY) return false;
	if(v1.cZ < v2.cZ) return true; if(v1.cZ > v2.cZ) return false;

	if(v1.nX < v2.nX) return true; if(v1.nX > v2.nX) return false;
	if(v1.nY < v2.nY) return true; if(v1.nY > v2.nY) return false;
	if(v1.nZ < v2.nZ) return true; if(v1.nZ > v2.nZ) return false;
	
	if(v1.hX < v2.hX) return true; if(v1.hX > v2.hX) return false;
	if(v1.hY < v2.hY) return true; if(v1.hY > v2.hY) return false;
	if(v1.hZ < v2.hZ) return true; if(v1.hZ > v2.hZ) return false;
	return false;
}

bool operator>(ClusterCoordinates v1, ClusterCoordinates v2){
	if(v1.cX > v2.cX) return true; if(v1.cX < v2.cX) return false;
	if(v1.cY > v2.cY) return true; if(v1.cY < v2.cY) return false;
	if(v1.cZ > v2.cZ) return true; if(v1.cZ < v2.cZ) return false;
	
	if(v1.nX > v2.nX) return true; if(v1.nX < v2.nX) return false;
	if(v1.nY > v2.nY) return true; if(v1.nY < v2.nY) return false;
	if(v1.nZ > v2.nZ) return true; if(v1.nZ < v2.nZ) return false;

	if(v1.hX > v2.hX) return true; if(v1.hX < v2.hX) return false;
	if(v1.hY > v2.hY) return true; if(v1.hY < v2.hY) return false;
	if(v1.hZ > v2.hZ) return true; if(v1.hZ < v2.hZ) return false;

	return false;
}

bool operator==(ClusterCoordinates v1, ClusterCoordinates v2){
	return  (v1.cX == v2.cX) &&
		(v1.cY == v2.cY) &&
		(v1.cZ == v2.cZ) &&

		(v1.nX == v2.nX) &&
		(v1.nY == v2.nY) &&
		(v1.nZ == v2.nZ) && 

		(v1.hX == v2.hX) &&
		(v1.hY == v2.hY) &&
		(v1.hZ == v2.hZ) ; 
}

ClusterCollection::ClusterCollection(double scale, bool reverse){
	// Expects scale to be (1/granularity)
	// Why? Because Zach
	mScale = scale;
	mReverse = reverse;
	mCluster = new TStorage<ClusterCoordinates,Polypeptide*>;
}

ClusterCollection::~ClusterCollection(){
	delete mCluster;
}

void ClusterCollection::createKey(Polypeptide *pp, ClusterCoordinates *key){
	if(!mReverse){
		Atom n = pp->getAminoAcid(pp->getResidueCount() - 1)->getAtom(7);
		Atom h = pp->getAminoAcid(pp->getResidueCount() - 1)->getAtom(8);
		Atom c = pp->getAminoAcid(pp->getResidueCount() - 1)->getAtom(9);
		
		key->nX = (int)(n.getCoordinate(0) * mScale);
		key->nY = (int)(n.getCoordinate(1) * mScale);
		key->nZ = (int)(n.getCoordinate(2) * mScale);

		key->hX = (int)(h.getCoordinate(0) * mScale);
		key->hY = (int)(h.getCoordinate(1) * mScale);
		key->hZ = (int)(h.getCoordinate(2) * mScale);

		key->cX = (int)(c.getCoordinate(0) * mScale);
		key->cY = (int)(c.getCoordinate(1) * mScale);
		key->cZ = (int)(c.getCoordinate(2) * mScale);

	}else{
		Atom n = pp->getAminoAcid(0)->getAtom(0);
		Atom h = pp->getAminoAcid(0)->getAtom(1);
		Atom c = pp->getAminoAcid(0)->getAtom(2);

		key->nX = (int)(n.getCoordinate(0) * mScale);
		key->nY = (int)(n.getCoordinate(1) * mScale);
		key->nZ = (int)(n.getCoordinate(2) * mScale);

		key->hX = (int)(h.getCoordinate(0) * mScale);
		key->hY = (int)(h.getCoordinate(1) * mScale);
		key->hZ = (int)(h.getCoordinate(2) * mScale);
		
		key->cX = (int)(c.getCoordinate(0) * mScale);
		key->cY = (int)(c.getCoordinate(1) * mScale);
		key->cZ = (int)(c.getCoordinate(2) * mScale);
	}
}

void ClusterCollection::addPolypeptide(Polypeptide *pp, double score){
	ClusterCoordinates key;
	createKey(pp, &key);
	key.rmsd = score;

	TStorageNode<ClusterCoordinates,Polypeptide*> *node = mCluster->Find(key);

	if(
		node == NULL ||
		score < node->GetKey().rmsd ||
		(score == node->GetKey().rmsd && pp->id < node->GetData()->id)
		){
		if (node != NULL) {
			delete node->GetData();
			mCluster->Delete(node);
		}
		Polypeptide *n_pp = new Polypeptide(*pp);
		mCluster->Insert(key, n_pp);
	}
}

double ClusterCollection::getClusterRMSD(Polypeptide *pp){
	ClusterCoordinates key;
	createKey(pp, &key);

	TStorageNode<ClusterCoordinates,Polypeptide*> *node = mCluster->Find(key);
	if(node != NULL){
		return node->GetKey().rmsd;
	}

	return 65000.0;
}

/**bool ClusterCollection::removePolypeptide(Polypeptide *pp){
  ClusterCoordinates key;
  key.cX = (int)(pp->getAminoAcid(pp->getResidueCount() - 2)->getAtom(2).getCoordinate(0) * mScale);
  key.cY = (int)(pp->getAminoAcid(pp->getResidueCount() - 2)->getAtom(2).getCoordinate(1) * mScale);
  key.cZ = (int)(pp->getAminoAcid(pp->getResidueCount() - 2)->getAtom(2).getCoordinate(2) * mScale);
  key.nX = (int)(pp->getAminoAcid(pp->getResidueCount() - 2)->getAtom(0).getCoordinate(0) * mScale);
  key.nY = (int)(pp->getAminoAcid(pp->getResidueCount() - 2)->getAtom(0).getCoordinate(1) * mScale);
  key.nZ = (int)(pp->getAminoAcid(pp->getResidueCount() - 2)->getAtom(0).getCoordinate(2) * mScale);

  TStorageNode<ClusterCoordinates,Polypeptide*> *node = mCluster->Find(key);
  if(node != NULL){
  mCluster->Delete(node);
  return true;
  }

  return false;
  }*/

bool ClusterCollection::clusterExists(Polypeptide *pp){
	ClusterCoordinates key;
	createKey(pp, &key);

	TStorageNode<ClusterCoordinates,Polypeptide*> *node = mCluster->Find(key);
	if(node != NULL)
		return true;
	return false;
}

void ClusterCollection::clear(){
	mCluster->Clear();
}

TStorageNode<ClusterCoordinates,Polypeptide*>* ClusterCollection::getNode(){
	return mCluster->GetFirst();
}
