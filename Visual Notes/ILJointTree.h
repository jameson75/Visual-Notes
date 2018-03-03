
class CILJointTree : public CILJoint
{
protected:
	static int m_ForkDistance;
	
public:
	CILJointTree();
	CILJointTree( CPoint ptOrigin );
	BOOL GetUpdateRect( CRect *pRect );
	BOOL HitTest( CPoint CanvasPoint );

protected:
	void _RenderJointEdge( CDC *pDC );
	void _New( CILSymbol **ppSymbol );
		
	
	DECLARE_SERIAL( CILJointTree )
};
