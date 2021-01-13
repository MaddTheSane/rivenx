//
//  RXErrors.h
//  rivenx
//

#if !defined(RX_ERRORS_H)
#define RX_ERRORS_H

#import "Base/RXBase.h"
#import <Foundation/NSError.h>

__BEGIN_DECLS

extern NSErrorDomain const RXErrorDomain;
typedef NS_ERROR_ENUM(RXErrorDomain, RXErrors) {
  kRXErrSavedGameCantBeLoaded,
  kRXErrQuickTimeTooOld,
  kRXErrArchivesNotFound,
  kRXErrFailedToInitializeStack,
  kRXErrFailedToGetDisplayID,
  kRXErrNoAcceleratorService,
  kRXErrFailedToGetAcceleratorPerfStats,
  kRXErrFailedToFindFreeVRAMInformation,
  kRXErrFailedToCreatePixelFormat,
  kRXErrInstallerAlreadyRan,
  kRXErrInstallerMissingArchivesOnMedia,
  kRXErrInstallerCancelled,
  kRXErrInstallerMissingArchivesAfterInstall,
  kRXErrFailedToGetFilesystemInformation,
  kRXErrUnusableInstallMedia,
  kRXErrUnusableInstallFolder,
  kRXErrInstallerGOGSetupUnpackFailed,
};


@interface RXError : NSError
@end


__END_DECLS

#endif // RX_ERRORS_H
