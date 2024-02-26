/* 
 * File:   cluster_collection.h
 * Author: Mikhail Simin, Zach Swearingen
 *
 * Created on May 27, 2010, 1:14 PM
 */

#ifndef _CLUSTER_COLLECTION_H
#define	_CLUSTER_COLLECTION_H

#include "polypeptide.h"
#include "tstorage.h"

struct ClusterCoordinates{
	int cX, cY, cZ;
	int nX, nY, nZ;
	int hX, hY, hZ;
	double rmsd;
};

class ClusterCollection{
	public:
		ClusterCollection(double scale, bool reverse = false);
		~ClusterCollection();

		void addPolypeptide(Polypeptide *pp, double score);
		bool removePolypeptide(Polypeptide *pp);
		bool clusterExists(Polypeptide *pp);
		TStorageNode<ClusterCoordinates,Polypeptide*>* getNode();
		double getClusterRMSD(Polypeptide *pp);

		int GetCount() { return mCluster->GetCount(); }

		double getScale(){return mScale;}    
		void setScale(double newScale){mScale = newScale;}


		void clear();

	private:
		void createKey(Polypeptide*, ClusterCoordinates*);

		TStorage<ClusterCoordinates,Polypeptide*> *mCluster;
		bool mReverse;
		double mScale;
};

#endif	/* _CLUSTER_COLLECTION_H */

