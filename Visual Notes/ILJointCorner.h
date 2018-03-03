
class CILJointCorner : public CILJoint
{
private:
	int m_nOutDistance;

public:
	CILJointCorner();
	CILJointCorner( CPoint ptOrigin );
	BOOL HitTest( CPoint CanvasPoint );
	
protected:
	void _RenderJointEdge( CDC *pDC );
	void _New( CILSymbol **ppSymbol );

	DECLARE_SERIAL( CILJointCorner );

};



