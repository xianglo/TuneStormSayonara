#ifndef _CUSTOMMIMEDATA_H
#define _CUSTOMMIMEDATA_H

#include "HelperStructs/MetaData.h"
#include <QMimeData>
#include <vector>

using namespace std;

class CustomMimeData : public QMimeData {
	
private:
	vector<MetaData> _v_md;

public:
	CustomMimeData() : QMimeData(){
	}

	virtual ~CustomMimeData(){}

	void setMetaData(const vector<MetaData>& v_md){
		_v_md = v_md;
	}

	uint getMetaData(vector<MetaData>& v_md) const {
		
		v_md = _v_md;
		return v_md.size();
	}
};


#endif
