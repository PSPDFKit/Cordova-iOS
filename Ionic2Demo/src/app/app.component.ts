import { Component } from '@angular/core';
import { Platform } from 'ionic-angular';
import { StatusBar } from '@ionic-native/status-bar';
import { SplashScreen } from '@ionic-native/splash-screen';
import { File as NativeFile} from '@ionic-native/file';

import { HomePage } from '../pages/home/home';


@Component({
  templateUrl: 'app.html'
})
export class MyApp {
  rootPage:any = HomePage;

  // Added Native file for PSPDFKit
  constructor(nativeFile: NativeFile, platform: Platform, statusBar: StatusBar, splashScreen: SplashScreen) {
    platform.ready().then(() => {
      // Okay, so the platform is ready and our plugins are available.
      // Here you can do any higher level native things you might need.
      statusBar.styleDefault();
      splashScreen.hide();

      // Added Native file for PSPDFKit
      console.debug('File applicationDirectory', nativeFile.applicationDirectory);
      console.debug('File documentsDirectory', nativeFile.documentsDirectory);
      console.debug('File dataDirectory', nativeFile.dataDirectory);

      nativeFile.getFreeDiskSpace().then(free => console.debug('Free disk space', free));

      // set your license key here
      PSPDFKitPlugin.setLicenseKey(
        "YOURLICENCEHERE"
      );

      PSPDFKitPlugin.present(nativeFile.applicationDirectory + 'www/assets/pdf/PSPDFKit 6 QuickStart Guide.pdf', {
        pageTransition: 'curl',
        backgroundColor: 'white',
      });
    });
  }
}
