#ifndef _TOOL_H_
#define _TOOL_H_

FS_BOOL CustomToolOnInit(FS_LPVOID clientData);
FS_BOOL CustomToolDestroy(FS_LPVOID clientData);
void	CustomToolOnActivate(FS_LPVOID clientData, FS_BOOL bPersistent);
void	CustomToolOnDeactivate(FS_LPVOID clientData);
FS_BOOL CustomToolOnKeyDown(FS_LPVOID clientData, FS_UINT nKeyCode, FS_UINT nFlags);
FS_BOOL CustomToolOnKeyUp(FS_LPVOID clientData, FS_UINT nKeyCode, FS_UINT nFlags);
FS_BOOL CustomToolOnChar(FS_LPVOID clientData, FS_UINT nChar, FS_UINT nFlags);
void	CustomToolOnLeavePage(FS_LPVOID clientData, FR_PageView pageview);
FS_BOOL CustomToolIsEnabled(FS_LPVOID clientData);
FS_BOOL CustomToolOnLButtonDown(FS_LPVOID clientData, FR_PageView pageview, FS_UINT nFlags, FS_DevicePoint point);
FS_BOOL CustomToolOnLButtonUp(FS_LPVOID clientData, FR_PageView pageview, FS_UINT nFlags, FS_DevicePoint point);
FS_BOOL CustomToolOnLButtonDblClk(FS_LPVOID clientData, FR_PageView pageview, FS_UINT nFlags, FS_DevicePoint point);
FS_BOOL CustomToolOnMouseMove(FS_LPVOID clientData, FR_PageView pageview, FS_UINT nFlags, FS_DevicePoint point);
FS_BOOL CustomToolOnRButtonDown(FS_LPVOID clientData, FR_PageView pageview, FS_UINT nFlags, FS_DevicePoint point);
FS_BOOL CustomToolOnRButtonUp(FS_LPVOID clientData, FR_PageView pageview, FS_UINT nFlags, FS_DevicePoint point);
FS_BOOL CustomToolOnRButtonDblClk(FS_LPVOID clientData, FR_PageView pageview, FS_UINT nFlags, FS_DevicePoint point);
FS_BOOL CustomToolOnMouseWheel(FS_LPVOID clientData, FR_PageView pageview, FS_UINT nFlags, FS_SHORT zDelta, FS_DevicePoint point);
FS_BOOL CustomToolOnDraw(FS_LPVOID clientData, FR_WinPort winPort, FS_DWORD dwFlags);
FS_BOOL	CustomToolIsProcessing(FS_LPVOID clientData);
FS_BOOL	CustomToolOnMouseHover(FS_LPVOID clientData, FR_PageView pageview, FS_DevicePoint point);
FS_BOOL	CustomToolIsWndCapturing(FS_LPVOID clientData);

/* @CALLBACKEND */
#endif //_TOOL_H_