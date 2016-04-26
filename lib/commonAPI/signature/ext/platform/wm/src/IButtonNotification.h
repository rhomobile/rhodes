class IButtonNotification
{
public:
	const static int ID_CLEAR = 1;
	const static int ID_CAPTURE = 2;
	const static int ID_CANCEL = 3;

	virtual void OnClick (int id) = 0;
	virtual void MUP() = 0;
	virtual ~IButtonNotification() {}
};