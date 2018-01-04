cordova.define('cordova/plugin_list', function(require, exports, module) {
module.exports = [
    {
        "id": "pspdfkit-cordova-ios.PSPDFKitPlugin",
        "file": "plugins/pspdfkit-cordova-ios/PSPDFKitPlugin/pspdfkit.js",
        "pluginId": "pspdfkit-cordova-ios",
        "clobbers": [
            "PSPDFKitPlugin"
        ]
    }
];
module.exports.metadata = 
// TOP OF METADATA
{
    "cordova-plugin-whitelist": "1.0.0",
    "pspdfkit-cordova-ios": "1.2.0"
};
// BOTTOM OF METADATA
});