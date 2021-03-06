/****************************************************************************
 *
 * $Id: testFeatureMoment.cpp 3323 2011-09-13 15:23:56Z fnovotny $
 *
 * This file is part of the ViSP software.
 * Copyright (C) 2005 - 2013 by INRIA. All rights reserved.
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * ("GPL") version 2 as published by the Free Software Foundation.
 * See the file LICENSE.txt at the root directory of this source
 * distribution for additional information about the GNU GPL.
 *
 * For using ViSP with software that can not be combined with the GNU
 * GPL, please contact INRIA about acquiring a ViSP Professional
 * Edition License.
 *
 * See http://www.irisa.fr/lagadic/visp/visp.html for more information.
 *
 * This software was developed at:
 * INRIA Rennes - Bretagne Atlantique
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * France
 * http://www.irisa.fr/lagadic
 *
 * If you have questions regarding the use of this file, please contact
 * INRIA at visp@inria.fr
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *
 * Description:
 * Example of visual servoing with moments using a polygon as object container
 *
 * Authors:
 * Filip Novotny
 *
 *****************************************************************************/
#include <visp/vpMomentObject.h>
#include <visp/vpMomentDatabase.h>
#include <visp/vpMomentCommon.h>
#include <visp/vpFeatureMomentCommon.h>
#include <visp/vpHomogeneousMatrix.h>
#include <visp/vpServo.h>
#include <visp/vpDebug.h>
#include <visp/vpPlane.h>

#include <limits>
#include <iostream>

//initialize scene in the interface
void initScene();

void init(vpHomogeneousMatrix& cMo, vpHomogeneousMatrix& cdMo);
vpMatrix execute(); //launch the test
void _planeToABC(vpPlane& pl, double& A,double& B, double& C);
int test(double x,double y,double z,double alpha);

//start and destination positioning matrices
vpHomogeneousMatrix cMo;
vpHomogeneousMatrix cdMo;

vpServo::vpServoIteractionMatrixType interaction_type; //current or desired

//source and destination objects for moment manipulation
vpMomentObject src(6);
vpMomentObject dst(6);

using namespace std;

//Compute a set of parallel positions and check if the matrix is in the right form;
int main()
{
  int sum=0;
  for(double i=-0.2;i<0.2;i+=0.1){
    for(double j=-0.2;j<0.2;j+=0.1){
      for(double k=-vpMath::rad(30);k<vpMath::rad(30);k+=vpMath::rad(10)){
        for(double l=0.5;l<1.5;l+=0.1){
          sum+=test(i,j,l,k);
        }
      }

    }
  }


  if(sum<0) return -1;
  else return 0;
}

int test(double x,double y,double z,double alpha){
  //intial pose
  vpHomogeneousMatrix cMo(x,y,z,-vpMath::rad(0),vpMath::rad(0),alpha);
  //Desired pose
  vpHomogeneousMatrix cdMo(vpHomogeneousMatrix(0.0,0.0,1.0,vpMath::rad(0),vpMath::rad(0),-vpMath::rad(0)));

  //init and run the simulation
  init(cMo,cdMo);
  vpMatrix mat = execute();

  if(fabs(mat[0][0]-(-1)) > numeric_limits<double>::epsilon()*1e10) return -1;
  if(fabs(mat[0][1]-(0)) > numeric_limits<double>::epsilon()*1e10) return -1;
  if(fabs(mat[0][2]-(0)) > numeric_limits<double>::epsilon()*1e10) return -1;

  if(fabs(mat[1][0]-(0)) > numeric_limits<double>::epsilon()*1e10) return -1;
  if(fabs(mat[1][1]-(-1)) > numeric_limits<double>::epsilon()*1e10) return -1;
  if(fabs(mat[1][2]-(0)) > numeric_limits<double>::epsilon()*1e10) return -1;

  if(fabs(mat[2][0]-(0)) > numeric_limits<double>::epsilon()*1e10) return -1;
  if(fabs(mat[2][1]-(0)) > numeric_limits<double>::epsilon()*1e10) return -1;
  if(fabs(mat[2][2]-(-1)) > numeric_limits<double>::epsilon()*1e10) return -1;
  if(fabs(mat[2][5]-(0)) > numeric_limits<double>::epsilon()*1e10) return -1;

  if(fabs(mat[3][0]-(0)) > numeric_limits<double>::epsilon()*1e10) return -1;
  if(fabs(mat[3][1]-(0)) > numeric_limits<double>::epsilon()*1e10) return -1;
  if(fabs(mat[3][2]-(0)) > numeric_limits<double>::epsilon()*1e10) return -1;
  if(fabs(mat[3][5]-(0)) > numeric_limits<double>::epsilon()*1e10) return -1;

  if(fabs(mat[4][0]-(0)) > numeric_limits<double>::epsilon()*1e10) return -1;
  if(fabs(mat[4][1]-(0)) > numeric_limits<double>::epsilon()*1e10) return -1;
  if(fabs(mat[4][2]-(0)) > numeric_limits<double>::epsilon()*1e10) return -1;
  if(fabs(mat[4][5]-(0)) > numeric_limits<double>::epsilon()*1e10) return -1;

  if(fabs(mat[5][0]-(0)) > numeric_limits<double>::epsilon()*1e10) return -1;
  if(fabs(mat[5][1]-(0)) > numeric_limits<double>::epsilon()*1e10) return -1;
  if(fabs(mat[5][2]-(0)) > numeric_limits<double>::epsilon()*1e10) return -1;
  if(fabs(mat[5][5]-(-1)) > numeric_limits<double>::epsilon()*1e10) return -1;

  return 0;
}





void initScene(){
  vector<vpPoint> src_pts;
  vector<vpPoint> dst_pts;

  double x[5] = { 0.2, 0.2,-0.2,-0.2, 0.2 };
  double y[5] = {-0.1, 0.1, 0.1,-0.1,-0.1 };
  int nbpoints = 4;

  for (int i = 0 ; i < nbpoints ; i++){
    vpPoint p;
    p.setWorldCoordinates(x[i],y[i],0.0);
    p.track(cMo) ;
    src_pts.push_back(p);
  }

  src.setType(vpMomentObject::DENSE_POLYGON);
  src.fromVector(src_pts);
  for (int i = 0 ; i < nbpoints ; i++){
    vpPoint p;
    p.setWorldCoordinates(x[i],y[i],0.0);
    p.track(cdMo) ;
    dst_pts.push_back(p);
  }
  dst.setType(vpMomentObject::DENSE_POLYGON);
  dst.fromVector(dst_pts);

}


void init(vpHomogeneousMatrix& _cMo, vpHomogeneousMatrix& _cdMo)
{
  cMo = _cMo;
  cdMo = _cdMo;
  interaction_type = vpServo::CURRENT;


  initScene(); //initialize graphical scene (for interface)
}

vpMatrix execute(){
  vpServo task;
  task.setServo(vpServo::EYEINHAND_CAMERA);
  //A,B,C parameters of source and destination plane
  double A; double B; double C;
  double Ad; double Bd; double Cd;
  //init main object: using moments up to order 6

  //Initializing values from regular plane (with ax+by+cz=d convention)
  vpPlane pl;
  pl.setABCD(0,0,1.0,0);
  pl.changeFrame(cMo);
  _planeToABC(pl,A,B,C);

  pl.setABCD(0,0,1.0,0);
  pl.changeFrame(cdMo);
  _planeToABC(pl,Ad,Bd,Cd);

  //extracting initial position (actually we only care about Zdst)
  vpTranslationVector vec;
  cdMo.extract(vec);

  ///////////////////////////// initializing moments and features /////////////////////////////////
  //don't need to be specific, vpMomentCommon automatically loads Xg,Yg,An,Ci,Cj,Alpha moments
  vpMomentCommon moments (vpMomentCommon ::getSurface(dst),vpMomentCommon::getMu3(dst),vpMomentCommon::getAlpha(dst), vec[2]);
  vpMomentCommon momentsDes(vpMomentCommon::getSurface(dst),vpMomentCommon::getMu3(dst),vpMomentCommon::getAlpha(dst),vec[2]);
  //same thing with common features
  vpFeatureMomentCommon featureMoments(moments);
  vpFeatureMomentCommon featureMomentsDes(momentsDes);

  moments.updateAll(src);
  momentsDes.updateAll(dst);

  featureMoments.updateAll(A,B,C);
  featureMomentsDes.updateAll(Ad,Bd,Cd);

  //setup the interaction type
  task.setInteractionMatrixType(interaction_type) ;
  //////////////////////////////////add useful features to task//////////////////////////////
  task.addFeature(featureMoments.getFeatureGravityNormalized(),featureMomentsDes.getFeatureGravityNormalized());
  task.addFeature(featureMoments.getFeatureAn(),featureMomentsDes.getFeatureAn());
  //the moments are different in case of a symmetric object
  task.addFeature(featureMoments.getFeatureCInvariant(),featureMomentsDes.getFeatureCInvariant(),(1 << 10) | (1 << 11));
  task.addFeature(featureMoments.getFeatureAlpha(),featureMomentsDes.getFeatureAlpha());

  task.setLambda(0.4) ;

  task.computeControlLaw();
  vpMatrix mat = task.computeInteractionMatrix();
  task.kill();
  return mat;
}


void _planeToABC(vpPlane& pl, double& A,double& B, double& C){

  A=-pl.getA()/pl.getD();
  B=-pl.getB()/pl.getD();
  C=-pl.getC()/pl.getD();
}

