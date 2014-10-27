#include "drpg_glModel.h"
using namespace drpg;

drModel::drModel()
{
	AMOUNT_GROUP = 0;
	AMOUNT_GROUP_SPHERE = 0;

	sVec.change( 1.0, 1.0, 1.0 );
	tVec.change( 0.0, 0.0, 0.0 );
	rVec.change( 0.0, 0.0, 0.0 );
	modelMatrix = glm::mat4( 1 );
}

drModel::drModel( const char* filename )
{
	AMOUNT_GROUP = 0;
	AMOUNT_GROUP_SPHERE = 0;
	loadDrm( filename );

	sVec.change( 1.0, 1.0, 1.0 );
	tVec.change( 0.0, 0.0, 0.0 );
	rVec.change( 0.0, 0.0, 0.0 );
	modelMatrix = glm::mat4( 1 );
}

drModel::~drModel()
{

}

void drModel::draw( unsigned int programID, int textureMode, drTextureSource* textures, drTextureSource* normalTextures )
{
	glPushMatrix();
	glTranslated( tVec.x, tVec.y, tVec.z );
	glRotated( rVec.x, 1.0, 0.0, 0.0 );
	glRotated( rVec.y, 0.0, 1.0, 0.0 );
	glRotated( rVec.z, 0.0, 0.0, 1.0 );
	glScaled( sVec.x, sVec.y, sVec.z );
	modelMatrix = glm::translate( tVec.x, tVec.y, tVec.z ) *
		glm::rotate( rVec.x, 1.0, 0.0, 0.0 ) *		
		glm::rotate( rVec.y, 0.0, 1.0, 0.0 ) *
		glm::rotate( rVec.z, 0.0, 0.0, 1.0 ) *
		glm::scale( sVec.x, sVec.y, sVec.z );
	glUniformMatrix4fv( glGetUniformLocation( programID, "modelMatrix" ), 1, GL_FALSE,
		&modelMatrix[0][0] );

	// vertex buffer object test
	/*
	for( unsigned int i=0 ; i<object.size() ; i++ ){
	object[i].draw( programID, textureMode, textures, normalTextures );
	}
	*/
	for( int i=0 ; i<objList.size() ; i++ ){
		if( textureMode == DRM_ONETEXTURE ){
			glActiveTexture( GL_TEXTURE0 );
			glBindTexture(GL_TEXTURE_2D,textures->textureID[objList[i].texID]);			
			glUniform1i( glGetUniformLocation( programID, "tex" ), 0 );
		}
		else if( textureMode == DRM_NORMALMAPPING ){
			glActiveTexture( GL_TEXTURE0 );
			glBindTexture(GL_TEXTURE_2D,textures->textureID[objList[i].texID]);			
			glUniform1i( glGetUniformLocation( programID, "tex" ), 0 );
			glActiveTexture( GL_TEXTURE1 );
			glBindTexture(GL_TEXTURE_2D,normalTextures->textureID[objList[i].texID]);			
			glUniform1i( glGetUniformLocation( programID, "normaltex" ), 1 );
		}
		else if( textureMode == DRM_CUBEMAPPING ){						
			glActiveTexture( GL_TEXTURE0 );
			glBindTexture( GL_TEXTURE_CUBE_MAP, textures->textureID[0] );
			glUniform1i( glGetUniformLocation( programID, "cubeMap" ), 0 );
		}
		glCallList( objList[i].id );
	}

	glPopMatrix();
}

void drModel::rotateX( double angle )
{
	rVec.changeX( angle );
}

void drModel::rotateY( double angle )
{
	rVec.changeY( angle );
}

void drModel::rotateZ( double angle )
{
	rVec.changeZ( angle );
}

void drModel::translate( vector3 translateVec )
{
	tVec.change( translateVec );
}

void drModel::translate( double Tx, double Ty, double Tz )
{
	tVec.change( Tx, Ty, Tz );
}

void drModel::scale( vector3 scaleVec )
{
	sVec.change( scaleVec );
}

void drModel::scale( double Sx, double Sy, double Sz )
{
	sVec.change( Sx, Sy, Sz );
}

void drModel::loadDrm( const char* filename )
{
	Group* objectData;
	Group_Sphere* objectDataS;

	char line[128];
	int LOAD_OBJECT_FLAG = 0;

	std::fstream fp;
	fp.open( filename, std::ios::in );
	if( !fp ){
		MessageBox(NULL, _T("開啟/創建檔案失敗"), _T("警告"), MB_OK | MB_ICONINFORMATION);
	}
	// 邊界
	float temp_SITE[3][MAX_PLANE_OF_GROUP*MAX_VERTEX_OF_PLANE] = {0};	 // 用來存放每一Group中最大數量的點 
	int count_SITE[3] = {0};

	// 正方體
	float WHICH_LINE = 0;
	int COUNT_LINE_GROUP = 0;
	int COUNT_LINE_PLANE = 0;
	int COUNT_LINE_POINT = 0;

	//cout << endl;

	while( fp.getline(line, sizeof(line), '\n') ){
		if( strcmp( line, "cube" ) == 0 ){
			LOAD_OBJECT_FLAG = 0;
			continue;
		}
		else if( strcmp( line, "sphere" ) == 0 ){
			LOAD_OBJECT_FLAG = 1;
			WHICH_LINE = 0;
			COUNT_LINE_GROUP = 0;
			COUNT_LINE_PLANE = 0;
			COUNT_LINE_POINT = 0;
			for( int i=0 ; i<3 ; i++ ){
				for( int j=0 ; j<MAX_PLANE_OF_GROUP*MAX_VERTEX_OF_PLANE ; j++ ){
					temp_SITE[i][j] = 0;
				}
				count_SITE[i] = 0;
			}
			continue;
		}
		if( LOAD_OBJECT_FLAG == 0 ){
			if( WHICH_LINE == 0 ){
				AMOUNT_GROUP = atoi(line);
				if( AMOUNT_GROUP <= DRPGM_MAX_GROUP )
					objectData = new Group[AMOUNT_GROUP];
				else
					objectData = new Group[DRPGM_MAX_GROUP];

				//std::cout << AMOUNT_GROUP << std::endl;
				WHICH_LINE = 0.5;
				continue;
			}
			else if( WHICH_LINE == 0.5 ){
				WHICH_LINE = 1;
				continue;			
			}
			else if( WHICH_LINE == 1 ){
				objectData[COUNT_LINE_GROUP].AMOUNT_PLANE = atoi(line);
				//cout << World[COUNT_LINE_GROUP].AMOUNT_PLANE << endl;
				WHICH_LINE = 2;
				continue;
			}
			else if( WHICH_LINE == 2 ){
				objectData[COUNT_LINE_GROUP].Plane[COUNT_LINE_PLANE].AMOUNT_POINT = atoi(line);
				//cout << World[COUNT_LINE_GROUP].Plane[COUNT_LINE_PLANE].AMOUNT_POINT << endl;
				WHICH_LINE = 3;
				continue;
			}
			else if( WHICH_LINE == 3 ){
				objectData[COUNT_LINE_GROUP].Plane[COUNT_LINE_PLANE].TexID = atoi(line);
				//cout << World[COUNT_LINE_GROUP].Plane[COUNT_LINE_PLANE].TexID << endl;
				WHICH_LINE = 4;
				continue;
			}
			else if( WHICH_LINE == 4 ){			
				char num[15];
				for( int i=0, j=0, n=0 ; (unsigned int)i<strlen(line) ; i++){
					if( line[i] == '&' || line[i] == '#'){
						num[n] = '\0';
						objectData[COUNT_LINE_GROUP].Plane[COUNT_LINE_PLANE].TexCoordRate[j] = (float)atof( num );
						//cout << World[COUNT_LINE_GROUP].Plane[COUNT_LINE_PLANE].TexCoordRate[j] << endl;
						j++;

						n=0;
					}
					else{
						num[n] = line[i];
						n++;
					}
				}
				WHICH_LINE = 5;
				continue;
			}
			//cout << strlen(line) << endl;
			else if( WHICH_LINE == 5 ){
				char num[15];
				for( int i=0, j=0, o=0, l=0, t=0, n=0 ; (unsigned int)i<strlen(line) ; i++){
					if( line[i] == '&' || line[i] == '#'){
						num[n] = '\0';
						if( j == 0 ){	
							//cout << num << '&';
							objectData[COUNT_LINE_GROUP].Plane[COUNT_LINE_PLANE].TexCoord[COUNT_LINE_POINT][o] = (float)atof(num);
							objectData[COUNT_LINE_GROUP].Used = true;
							objectData[COUNT_LINE_GROUP].Plane[COUNT_LINE_PLANE].Used = true;
							objectData[COUNT_LINE_GROUP].Plane[COUNT_LINE_PLANE].Point[COUNT_LINE_POINT].Used = true;

							//cout/* << COUNT_LINE_GROUP << ", " << COUNT_LINE_PLANE << ", " << COUNT_LINE_POINT << " : "*/ << World[COUNT_LINE_GROUP].Plane[COUNT_LINE_PLANE].TexCoord[COUNT_LINE_POINT][o] << '&';
							o++;
						}
						else if( j == 1 ){
							switch(l){
							case 0:
								objectData[COUNT_LINE_GROUP].Plane[COUNT_LINE_PLANE].Point[COUNT_LINE_POINT].Site.x = (float)atof(num);
								break;
							case 1:
								objectData[COUNT_LINE_GROUP].Plane[COUNT_LINE_PLANE].Point[COUNT_LINE_POINT].Site.y = (float)atof(num);
								break;
							case 2:
								objectData[COUNT_LINE_GROUP].Plane[COUNT_LINE_PLANE].Point[COUNT_LINE_POINT].Site.z = (float)atof(num);
								break;
							}
							temp_SITE[l][count_SITE[l]] = (float)atof(num);
							count_SITE[l]++;
							//cout << World[COUNT_LINE_GROUP].Plane[COUNT_LINE_PLANE].Point[COUNT_LINE_POINT].Site[l] << '&';
							l++;
						}
						n=0;
						for( int k=0 ; k<15 ; k++ )
							num[k] = '\0';
						if( j == 2 )
							break;
						if( line[i] == '#' ){
							//cout << '#' << endl;
							j++;
						}
					}
					else{
						num[n] = line[i];
						n++;
					}
				}		
				if( COUNT_LINE_PLANE != objectData[COUNT_LINE_GROUP].AMOUNT_PLANE-1 && COUNT_LINE_POINT == objectData[COUNT_LINE_GROUP].Plane[COUNT_LINE_PLANE].AMOUNT_POINT-1 ){
					COUNT_LINE_PLANE++;
					COUNT_LINE_POINT = 0;
					WHICH_LINE = 2;
					//cout << endl;	

				}
				else if( COUNT_LINE_PLANE == objectData[COUNT_LINE_GROUP].AMOUNT_PLANE-1 && COUNT_LINE_POINT == objectData[COUNT_LINE_GROUP].Plane[COUNT_LINE_PLANE].AMOUNT_POINT-1 ){
					float max = 0, min = 0;
					float temp_O[3];
					temp_O[0] = 0.0f;
					temp_O[1] = 0.0f;
					temp_O[2] = 0.0f;

					COUNT_LINE_PLANE = 0;
					COUNT_LINE_POINT = 0;
					WHICH_LINE = 0.5;

					for( int i=0 ; i<3 ; i++ ){
						Sort(temp_SITE[i], count_SITE[i], max, min);
						switch(i){
						case 0:
							objectData[COUNT_LINE_GROUP].len.x = (max-min);
							objectData[COUNT_LINE_GROUP].O.x = (max+min)/2;
							break;
						case 1:
							objectData[COUNT_LINE_GROUP].len.y = (max-min);
							objectData[COUNT_LINE_GROUP].O.y = (max+min)/2;
							break;
						case 2:
							objectData[COUNT_LINE_GROUP].len.z = (max-min);
							objectData[COUNT_LINE_GROUP].O.z = (max+min)/2;
							break;
						}
						//cout << temp_O[i] << ", " << count_SITE[i] << '#';
					}
					count_SITE[0] = 0;
					count_SITE[1] = 0;
					count_SITE[2] = 0;

					COUNT_LINE_GROUP++;
					//cout << endl;
				}
				else{
					COUNT_LINE_POINT++;
					//cout << endl;
				}
			}
		}
		else if( LOAD_OBJECT_FLAG == 1 ){
			if( WHICH_LINE == 0 ){
				AMOUNT_GROUP_SPHERE = atoi(line);
				if( AMOUNT_GROUP_SPHERE <= DRPGM_MAX_GROUP_SPHERE )
					objectDataS = new Group_Sphere[AMOUNT_GROUP_SPHERE];
				else
					objectDataS = new Group_Sphere[DRPGM_MAX_GROUP_SPHERE];
				WHICH_LINE = 0.5;
				continue;
			}
			else if( WHICH_LINE == 0.5 ){
				WHICH_LINE = 1;
				continue;			
			}
			else if( WHICH_LINE == 1 ){
				objectDataS[COUNT_LINE_GROUP].AMOUNT_PLANE = atoi(line);
				//cout << World[COUNT_LINE_GROUP].AMOUNT_PLANE << endl;
				WHICH_LINE = 2;
				continue;
			}
			else if( WHICH_LINE == 2 ){
				objectDataS[COUNT_LINE_GROUP].Plane[COUNT_LINE_PLANE].AMOUNT_POINT = atoi(line);
				//cout << World[COUNT_LINE_GROUP].Plane[COUNT_LINE_PLANE].AMOUNT_POINT << endl;
				WHICH_LINE = 3;
				continue;
			}
			else if( WHICH_LINE == 3 ){
				objectDataS[COUNT_LINE_GROUP].Plane[COUNT_LINE_PLANE].TexID = atoi(line);
				//cout << World[COUNT_LINE_GROUP].Plane[COUNT_LINE_PLANE].TexID << endl;
				WHICH_LINE = 4;
				continue;
			}
			else if( WHICH_LINE == 4 ){			
				char num[15];
				for( int i=0, j=0, n=0 ; (unsigned int)i<strlen(line) ; i++){
					if( line[i] == '&' || line[i] == '#'){
						num[n] = '\0';
						objectDataS[COUNT_LINE_GROUP].Plane[COUNT_LINE_PLANE].TexCoordRate[j] = (float)atof(num);
						//cout << World[COUNT_LINE_GROUP].Plane[COUNT_LINE_PLANE].TexCoordRate[j] << endl;
						j++;

						n=0;
					}
					else{
						num[n] = line[i];
						n++;
					}
				}
				WHICH_LINE = 5;
				continue;
			}
			//cout << strlen(line) << endl;
			else if( WHICH_LINE == 5 ){
				char num[15];
				for( int i=0, j=0, o=0, l=0, t=0, n=0 ; (unsigned int)i<strlen(line) ; i++){
					if( line[i] == '&' || line[i] == '#'){
						num[n] = '\0';
						if( j == 0 ){	
							//cout << num << '&';
							objectDataS[COUNT_LINE_GROUP].Plane[COUNT_LINE_PLANE].TexCoord[COUNT_LINE_POINT][o] = (float)atof(num);
							objectDataS[COUNT_LINE_GROUP].Used = true;
							objectDataS[COUNT_LINE_GROUP].Plane[COUNT_LINE_PLANE].Used = true;
							objectDataS[COUNT_LINE_GROUP].Plane[COUNT_LINE_PLANE].Point[COUNT_LINE_POINT].Used = true;

							//cout/* << COUNT_LINE_GROUP << ", " << COUNT_LINE_PLANE << ", " << COUNT_LINE_POINT << " : "*/ << World[COUNT_LINE_GROUP].Plane[COUNT_LINE_PLANE].TexCoord[COUNT_LINE_POINT][o] << '&';
							o++;
						}
						else if( j == 1 ){
							switch(l){
							case 0:
								objectDataS[COUNT_LINE_GROUP].Plane[COUNT_LINE_PLANE].Point[COUNT_LINE_POINT].Site.x = (float)atof(num);
								break;
							case 1:
								objectDataS[COUNT_LINE_GROUP].Plane[COUNT_LINE_PLANE].Point[COUNT_LINE_POINT].Site.y = (float)atof(num);
								break;
							case 2:
								objectDataS[COUNT_LINE_GROUP].Plane[COUNT_LINE_PLANE].Point[COUNT_LINE_POINT].Site.z = (float)atof(num);
								break;
							}
							temp_SITE[l][count_SITE[l]] = (float)atof(num);
							count_SITE[l]++;
							//cout << World[COUNT_LINE_GROUP].Plane[COUNT_LINE_PLANE].Point[COUNT_LINE_POINT].Site[l] << '&';
							l++;
						}
						n=0;
						for( int k=0 ; k<15 ; k++ )
							num[k] = '\0';
						if( j == 2 )
							break;
						if( line[i] == '#' ){
							//cout << '#' << endl;
							j++;
						}
					}
					else{
						num[n] = line[i];
						n++;
					}
				}		
				if( COUNT_LINE_PLANE != objectDataS[COUNT_LINE_GROUP].AMOUNT_PLANE-1 && COUNT_LINE_POINT == objectDataS[COUNT_LINE_GROUP].Plane[COUNT_LINE_PLANE].AMOUNT_POINT-1 ){
					COUNT_LINE_PLANE++;
					COUNT_LINE_POINT = 0;
					WHICH_LINE = 2;
					//cout << endl;	

				}
				else if( COUNT_LINE_PLANE == objectDataS[COUNT_LINE_GROUP].AMOUNT_PLANE-1 && COUNT_LINE_POINT == objectDataS[COUNT_LINE_GROUP].Plane[COUNT_LINE_PLANE].AMOUNT_POINT-1 ){
					float max = 0, min = 0;
					float temp_O[3];
					temp_O[0] = 0.0f;
					temp_O[1] = 0.0f;
					temp_O[2] = 0.0f;

					COUNT_LINE_PLANE = 0;
					COUNT_LINE_POINT = 0;
					WHICH_LINE = 0.5;

					for( int i=0 ; i<3 ; i++ ){
						Sort(temp_SITE[i], count_SITE[i], max, min);
						switch(i){
						case 0:
							objectDataS[COUNT_LINE_GROUP].len.x = (max-min);
							objectDataS[COUNT_LINE_GROUP].O.x = (max+min)/2;
							break;
						case 1:
							objectDataS[COUNT_LINE_GROUP].len.y = (max-min);
							objectDataS[COUNT_LINE_GROUP].O.y = (max+min)/2;
							break;
						case 2:
							objectDataS[COUNT_LINE_GROUP].len.z = (max-min);
							objectDataS[COUNT_LINE_GROUP].O.z = (max+min)/2;
							break;
						}
						//cout << temp_O[i] << ", " << count_SITE[i] << '#';
					}
					count_SITE[0] = 0;
					count_SITE[1] = 0;
					count_SITE[2] = 0;

					COUNT_LINE_GROUP++;
					//cout << endl;
				}
				else{
					COUNT_LINE_POINT++;
					//cout << endl;
				}
			}
		}
	}
	fp.close();

	updateNvector( objectData, objectDataS );
	/*
	for( unsigned int i=0, j=0 ; i<AMOUNT_GROUP ; j++ ){
	if( objectData[j].Used ){
	i++;
	for( unsigned int k=0, l=0 ; k<objectData[j].AMOUNT_PLANE ; l++ ){
	if( objectData[j].Plane[l].Used ){
	k++;

	std::vector<vertexData> tmpvd;
	std::vector<unsigned int> tmpid;
	std::vector<textureData> tmptd;

	bool firstP = true;
	int fpIndex = 0;
	int countP = 0;
	for( unsigned int m=0, n=0 ; m<objectData[j].Plane[l].AMOUNT_POINT ; n++ ){
	if( objectData[j].Plane[l].Point[n].Used ){
	m++;

	vertexData tmp;
	tmp.position.change( objectData[j].Plane[l].Point[n].Site );
	tmp.normal.change( objectData[j].Plane[l].Nvector );
	tmp.U = objectData[j].Plane[l].TexCoord[n][0]*(sqrt(pow(objectData[j].Plane[l].Point[2].Site.x-objectData[j].Plane[l].Point[1].Site.x, 2) + pow(objectData[j].Plane[l].Point[2].Site.y-objectData[j].Plane[l].Point[1].Site.y, 2) + pow(objectData[j].Plane[l].Point[2].Site.z-objectData[j].Plane[l].Point[1].Site.z, 2)))/objectData[j].Plane[l].TexCoordRate[0];
	tmp.V = objectData[j].Plane[l].TexCoord[n][1]*(sqrt(pow(objectData[j].Plane[l].Point[0].Site.x-objectData[j].Plane[l].Point[1].Site.x, 2) + pow(objectData[j].Plane[l].Point[0].Site.y-objectData[j].Plane[l].Point[1].Site.y, 2) + pow(objectData[j].Plane[l].Point[0].Site.z-objectData[j].Plane[l].Point[1].Site.z, 2)))/objectData[j].Plane[l].TexCoordRate[1];
	//std::cout << tmp.position.x << ", " << tmp.position.y << ", " << tmp.position.z << std::endl;

	tmpvd.push_back( tmp );
	if( firstP ){
	firstP = false;
	tmpid.push_back( n );
	fpIndex = n;
	}
	else{
	if( countP != 2 ){
	countP++;
	tmpid.push_back( n );
	}
	else{
	tmpid.push_back( fpIndex );
	tmpid.push_back( tmpid[tmpid.size()-2] );
	tmpid.push_back( n );
	}
	}
	}
	}
	textureData tmpTex;
	tmpTex.id = objectData[j].Plane[l].TexID;
	tmptd.push_back( tmpTex );
	/*
	for( int x=0 ; x<tmpid.size() ; x+=3 ){
	std::cout << tmpvd[tmpid[x]].position.x << ", " << tmpvd[tmpid[x]].position.y << ", " << tmpvd[tmpid[x]].position.z << std::endl
	<< tmpvd[tmpid[x+1]].position.x << ", " << tmpvd[tmpid[x+1]].position.y << ", " << tmpvd[tmpid[x+1]].position.z << std::endl
	<< tmpvd[tmpid[x+2]].position.x << ", " << tmpvd[tmpid[x+2]].position.y << ", " << tmpvd[tmpid[x+2]].position.z << std::endl
	<< tmpid[x] << ", " << tmpid[x+1] << ", " << tmpid[x+2] << std::endl;
	}

	object.push_back( drMesh( &tmpvd, &tmpid, &tmptd ) );
	}
	}
	}
	}
	for( unsigned int i=0, j=0 ; i<AMOUNT_GROUP_SPHERE ; j++ ){
	if( objectDataS[j].Used ){
	i++;
	for( unsigned int k=0, l=0 ; k<objectDataS[j].AMOUNT_PLANE ; l++ ){
	if( objectDataS[j].Plane[l].Used ){
	k++;

	std::vector<vertexData> tmpvd;
	std::vector<unsigned int> tmpid;
	std::vector<textureData> tmptd;

	bool firstP = true;
	int fpIndex = 0;
	int countP = 0;
	for( unsigned int m=0, n=0 ; m<objectDataS[j].Plane[l].AMOUNT_POINT ; n++ ){
	if( objectDataS[j].Plane[l].Point[n].Used ){
	m++;

	vertexData tmp;
	tmp.position.change( objectDataS[j].Plane[l].Point[n].Site );
	tmp.normal.change( objectDataS[j].Plane[l].Point[n].Nvector );
	tmp.U = objectDataS[j].Plane[l].TexCoord[n][0]*(sqrt(pow(objectDataS[j].Plane[l].Point[2].Site.x-objectDataS[j].Plane[l].Point[1].Site.x, 2) + pow(objectDataS[j].Plane[l].Point[2].Site.y-objectDataS[j].Plane[l].Point[1].Site.y, 2) + pow(objectDataS[j].Plane[l].Point[2].Site.z-objectDataS[j].Plane[l].Point[1].Site.z, 2)))/objectDataS[j].Plane[l].TexCoordRate[0];
	tmp.V = objectDataS[j].Plane[l].TexCoord[n][1]*(sqrt(pow(objectDataS[j].Plane[l].Point[0].Site.x-objectDataS[j].Plane[l].Point[1].Site.x, 2) + pow(objectDataS[j].Plane[l].Point[0].Site.y-objectDataS[j].Plane[l].Point[1].Site.y, 2) + pow(objectDataS[j].Plane[l].Point[0].Site.z-objectDataS[j].Plane[l].Point[1].Site.z, 2)))/objectDataS[j].Plane[l].TexCoordRate[1];  

	tmpvd.push_back( tmp );
	if( firstP ){
	firstP = false;
	tmpid.push_back( n );
	fpIndex = n;
	}
	else{
	if( countP != 2 ){
	countP++;
	tmpid.push_back( n );
	}
	else{
	tmpid.push_back( fpIndex );
	tmpid.push_back( tmpid.size()-1 );
	tmpid.push_back( n );
	}
	}
	}
	}
	textureData tmpTex;
	tmpTex.id = objectData[j].Plane[l].TexID;
	tmptd.push_back( tmpTex );

	object.push_back( drMesh( &tmpvd, &tmpid, &tmptd ) );
	}
	}
	}
	}
	*/




	for( int i=0, j=0 ; i<AMOUNT_GROUP ; j++ ){
		if( objectData[j].Used ){
			i++;

			unsigned int tmpID = 0;
			tmpID = glGenLists( 1 );
			glNewList( tmpID, GL_COMPILE );
			planeEqu tmp;
			for( int k=0, l=0 ; k<objectData[j].AMOUNT_PLANE ; l++){
				if( objectData[j].Plane[l].Used ){		
					k++;


					glBegin(GL_POLYGON); 
					glNormal3d( objectData[j].Plane[l].Nvector.x, objectData[j].Plane[l].Nvector.y, objectData[j].Plane[l].Nvector.z );
					tmp.coe.push_back( objectData[j].Plane[l].Nvector );
					tmp.result.push_back( objectData[j].Plane[l].Point[0].Site );

					for( int m=0, n=0 ; m<objectData[j].Plane[l].AMOUNT_POINT ; n++ ){
						if( objectData[j].Plane[l].Point[n].Used ){
							m++;

							glTexCoord2d(objectData[j].Plane[l].TexCoord[n][0]*(sqrt(pow(objectData[j].Plane[l].Point[2].Site.x-objectData[j].Plane[l].Point[1].Site.x, 2) + pow(objectData[j].Plane[l].Point[2].Site.y-objectData[j].Plane[l].Point[1].Site.y, 2) + pow(objectData[j].Plane[l].Point[2].Site.z-objectData[j].Plane[l].Point[1].Site.z, 2)))/objectData[j].Plane[l].TexCoordRate[0], 
								objectData[j].Plane[l].TexCoord[n][1]*(sqrt(pow(objectData[j].Plane[l].Point[0].Site.x-objectData[j].Plane[l].Point[1].Site.x, 2) + pow(objectData[j].Plane[l].Point[0].Site.y-objectData[j].Plane[l].Point[1].Site.y, 2) + pow(objectData[j].Plane[l].Point[0].Site.z-objectData[j].Plane[l].Point[1].Site.z, 2)))/objectData[j].Plane[l].TexCoordRate[1]);  
							glVertex3d( objectData[j].Plane[l].Point[n].Site.x,
								objectData[j].Plane[l].Point[n].Site.y,
								objectData[j].Plane[l].Point[n].Site.z);  							
						}
					}
					glEnd(); 

				}		
			}
			glEndList();
			planeData tmpTex = { tmpID, objectData[j].Plane[0].TexID };
			objList.push_back( tmpTex );
			objEqu.push_back( tmp );
		}
	}	
	for( int i=0, j=0 ; i<AMOUNT_GROUP_SPHERE ; j++ ){
		if( objectDataS[j].Used ){
			i++;
			unsigned int tmpID = 0;
			tmpID = glGenLists( 1 );
			glNewList( tmpID, GL_COMPILE );
			for( int k=0, l=0 ; k<objectDataS[j].AMOUNT_PLANE ; l++){
				if( objectDataS[j].Plane[l].Used ){		
					k++;


					glBegin(GL_POLYGON); 

					for( int m=0, n=0 ; m<objectDataS[j].Plane[l].AMOUNT_POINT ; n++ ){
						if( objectDataS[j].Plane[l].Point[n].Used ){
							m++;

							glNormal3d( objectDataS[j].Plane[l].Point[n].Nvector.x, objectDataS[j].Plane[l].Point[n].Nvector.y, objectDataS[j].Plane[l].Point[n].Nvector.z );	
							glTexCoord2d(objectDataS[j].Plane[l].TexCoord[n][0]*(sqrt(pow(objectDataS[j].Plane[l].Point[2].Site.x-objectDataS[j].Plane[l].Point[1].Site.x, 2) + pow(objectDataS[j].Plane[l].Point[2].Site.y-objectDataS[j].Plane[l].Point[1].Site.y, 2) + pow(objectDataS[j].Plane[l].Point[2].Site.z-objectDataS[j].Plane[l].Point[1].Site.z, 2)))/objectDataS[j].Plane[l].TexCoordRate[0], 
								objectDataS[j].Plane[l].TexCoord[n][1]*(sqrt(pow(objectDataS[j].Plane[l].Point[0].Site.x-objectDataS[j].Plane[l].Point[1].Site.x, 2) + pow(objectDataS[j].Plane[l].Point[0].Site.y-objectDataS[j].Plane[l].Point[1].Site.y, 2) + pow(objectDataS[j].Plane[l].Point[0].Site.z-objectDataS[j].Plane[l].Point[1].Site.z, 2)))/objectDataS[j].Plane[l].TexCoordRate[1]);  
							glVertex3d( objectDataS[j].Plane[l].Point[n].Site.x,
								objectDataS[j].Plane[l].Point[n].Site.y,
								objectDataS[j].Plane[l].Point[n].Site.z); 
						}
					}
					glEnd(); 

				}		
			}
			glEndList();
			planeData tmpTex = { tmpID, objectDataS[j].Plane[0].TexID };
			objList.push_back( tmpTex );
		}
	}

	delete [] objectData;
	delete [] objectDataS;
}

void drModel::updateNvector( Group* obj, Group_Sphere* objS )
{
	for( int i=0, j=0 ; i<AMOUNT_GROUP ; j++ ){
		if( obj[j].Used ){
			i++;
			for( int k=0, l=0 ; k<obj[j].AMOUNT_PLANE ; l++ ){
				if( obj[j].Plane[l].Used ){
					k++;

					double temp_POINT[3][3];
					for( int m=0, n=0 ; m<3 ; n++ ){
						if( obj[j].Plane[l].Point[n].Used ){
							temp_POINT[m][0] = obj[j].Plane[l].Point[n].Site.x;
							temp_POINT[m][1] = obj[j].Plane[l].Point[n].Site.y;
							temp_POINT[m][2] = obj[j].Plane[l].Point[n].Site.z;
							m++;
						}
					}

					double temp_VECTOR[2][3];					
					temp_VECTOR[0][0] = temp_POINT[1][0]-temp_POINT[0][0];
					temp_VECTOR[0][1] = temp_POINT[1][1]-temp_POINT[0][1];
					temp_VECTOR[0][2] = temp_POINT[1][2]-temp_POINT[0][2];
					temp_VECTOR[1][0] = temp_POINT[2][0]-temp_POINT[0][0];
					temp_VECTOR[1][1] = temp_POINT[2][1]-temp_POINT[0][1];
					temp_VECTOR[1][2] = temp_POINT[2][2]-temp_POINT[0][2];

					vector3 tmp;
					tmp.x = (temp_VECTOR[0][2]*temp_VECTOR[1][1]-temp_VECTOR[0][1]*temp_VECTOR[1][2])*-1;
					tmp.y = (temp_VECTOR[0][0]*temp_VECTOR[1][2]-temp_VECTOR[0][2]*temp_VECTOR[1][0])*-1;
					tmp.z = (temp_VECTOR[0][1]*temp_VECTOR[1][0]-temp_VECTOR[0][0]*temp_VECTOR[1][1])*-1;

					tmp.normalize();
					obj[j].Plane[l].Nvector.change( tmp );
					obj[j].Plane[l].Equation = temp_POINT[0][0]*obj[j].Plane[l].Nvector.x + 
						temp_POINT[0][1]*obj[j].Plane[l].Nvector.y +
						temp_POINT[0][2]*obj[j].Plane[l].Nvector.z;
				}
			}
		}
	}
	for( int i=0, j=0 ; i<AMOUNT_GROUP_SPHERE ; j++ ){
		if( objS[j].Used ){
			i++;
			for( int k=0, l=0 ; k<objS[j].AMOUNT_PLANE ; l++ ){
				if( objS[j].Plane[l].Used ){
					k++;
					for( int m=0, n=0 ; m<objS[j].Plane[l].AMOUNT_POINT ; n++ ){
						if( objS[j].Plane[l].Point[n].Used ){
							m++;

							vector3 tmp;
							tmp = objS[j].Plane[l].Point[n].Site - objS[j].O;

							tmp.normalize();
							objS[j].Plane[l].Point[n].Nvector.change( tmp );
						}
					}
				}
			}
		}
	}
}

void drModel::Sort(float *list, int listcount, float &max, float &min)
{
	float tempL[MAX_SORT_LIST];
	for(int i=0 ; i<listcount ; i++)
		tempL[i] = list[i];
	for(int i=listcount ; i>0 ; i--){
		for(int j=0 ; j<i ; j++){
			if( tempL[j] < tempL[j+1] ){
				float temp = tempL[j];
				tempL[j] = tempL[j+1];
				tempL[j+1] = temp;
			}
		}
	}
	max = tempL[0];
	min = tempL[listcount-1];
}

void drModel::updateState( vector3 translateVec, vector3 scaleVec, vector3 rotateVec )
{
	tVec += translateVec;
	sVec += scaleVec;
	rVec += rotateVec;
	if( rVec.x >= 360 )
		rVec.x -= 360;
	if( rVec.y >= 360 )
		rVec.y -= 360;
	if( rVec.z >= 360 )
		rVec.z -= 360;
	if( rVec.x <= 0 )
		rVec.x += 360;
	if( rVec.y <= 0 )
		rVec.y += 360;
	if( rVec.z <= 0 )
		rVec.z += 360;
}

bool drModel::collisionDetect( vector3 subject )
{
	modelMatrix = glm::translate( tVec.x, tVec.y, tVec.z ) *
		glm::rotate( rVec.x, 1.0, 0.0, 0.0 ) *		
		glm::rotate( rVec.y, 0.0, 1.0, 0.0 ) *
		glm::rotate( rVec.z, 0.0, 0.0, 1.0 ) *
		glm::scale( sVec.x, sVec.y, sVec.z );
	glm::mat3 normalMatrix = glm::mat3( modelMatrix );

	bool POINT_INSIDE_FLAG = true;
	for( unsigned int i=0 ; i<objEqu.size() ; i++ ){
		POINT_INSIDE_FLAG = true;
		for( unsigned int j=0 ; j<objEqu[i].coe.size() ; j++ ){
			glm::vec3 tmpVecCoe = glm::vec3( objEqu[i].coe[j].x, objEqu[i].coe[j].y, objEqu[i].coe[j].z );
			glm::vec4 tmpVecResult = glm::vec4( objEqu[i].result[j].x, objEqu[i].result[j].y, objEqu[i].result[j].z, 1.0 );
			tmpVecCoe = normalMatrix * tmpVecCoe;
			tmpVecResult = modelMatrix * tmpVecResult;

			double tmpEqu = vector3( tmpVecCoe.x, tmpVecCoe.y, tmpVecCoe.z ).dot( subject );
			double tmpResult = vector3( tmpVecCoe.x, tmpVecCoe.y, tmpVecCoe.z ).dot( vector3( tmpVecResult.x, tmpVecResult.y, tmpVecResult.z ) );
			if( tmpEqu > tmpResult ){
				//std::cout << tmpEqu << ", " << tmpResult << std::endl;
				POINT_INSIDE_FLAG = false;
				break;
			}
		}
		if( POINT_INSIDE_FLAG )
			break;
	}
	if( POINT_INSIDE_FLAG )
		return true;
	return false;
}

vector3 drModel::getTstate()
{
	return tVec;
}
vector3 drModel::getRstate()
{
	return rVec;
}
vector3 drModel::getSstate()
{
	return sVec;
}

int drModel::getObjAmount()
{
	return objEqu.size();
}