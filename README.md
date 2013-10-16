This module loads CallFire's PHP SDK into Drupal, and provides a configuration interface and helper functions for creating REST and SOAP clients.

## Installation

Releases of this module are available through the [GitHub repository](https://github.com/CallFire/CallFire-Drupal-Integration/releases) as zipballs.

Copy the link to the version of the library you would like to install, and paste this in the appropriate text entry on the module installation page of your Drupal project.

Alternatively, download and extract the zip file into the sites/all/modules/ directory of your Drupal project.

## Configuration

The SDK client can be configured from the administration interface of your Drupal project by navigating to `Administration -> Configuration -> Web Services -> CallFire`.

From there, you can enter your API login and password.

## Basic Usage

Having successfully configured your API credentials in your Drupal installation, you can make use of the `callfire_sdk_rest_client` and `callfire_sdk_soap_client` helpers to create REST or SOAP clients, respectively. The first parameter to either of these functions is the type of client you wish to create, e.g.:

```php
$client = callfire_sdk_rest_client('Broadcast');
$request = $client::request('QueryBroadcasts');
$response = $client->QueryBroadcasts($request);
$broadcasts = $client::response($response);

foreach($broadcasts as $broadcast) {
    ...
}
```
