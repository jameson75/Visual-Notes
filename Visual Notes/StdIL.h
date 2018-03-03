#define VNRGBWORD(r,g,b) ( (r << 10) | (g << 5) | b )
#define VNRGBDWORD(r,g,b)( (r << 16) | (g << 8) | b )

#include "ILList.h"
#include "ILBitOp.h"

#include "ILSymbol.h"

#include "ILSymbolRectTracker.h"
#include "ILDragRect.h"

#include "ILShape.h"
#include "ILBitmapShape.h"

#include "ILLabel.h"

#include "ILJoint.h"
#include "ILJointTree.h"
#include "ILJointCorner.h"

#include "ILPolygon.h"

#include "ILRectangle.h"
#include "ILShear.h"
#include "ILCircle.h"
#include "ILDiamond.h"
#include "Generalize.h"
#include "ILTagA.h"
#include "ILTagB.h"

#include "ILUmlActor.h"
#include "ILUmlPackage.h"
#include "ILUmlNote.h"
#include "ILUmlComponent.h"
#include "ILUmlClass.h"
#include "ILRoundRect.h"
#include "ILUmlDeployment.h"
#include "ILCapsule.h"

#include "ILNetClient.h"
#include "ILNetServer.h"
#include "ILNetDatabase.h"
#include "ILNetCloud.h"
#include "ILNetPrinter.h"
#include "ILNetMainframe.h"
#include "ILNetSatellite.h"
#include "ILNetDish.h"
#include "ILNetCellphone.h"
#include "ILNetPDA.h"
#include "ILNetCamera.h"
#include "ILNetFax.h"
#include "ILNetThinkpad.h"
#include "ILNetModem.h"
#include "ILNetHub.h"
#include "ILNetRing.h"

#include "ILTransaction.h"
#include "ILTransactionCoord.h"
#include "ILSymbolDB.h"







