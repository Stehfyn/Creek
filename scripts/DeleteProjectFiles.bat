del "..\WalnutApp.sln"
rd /s /q "..\bin-int"

del "..\Walnut\Walnut.vcxproj"
del "..\Walnut\Walnut.vcxproj.user"
del "..\Walnut\Walnut.vcxproj.filters"
rd /s /q "..\Walnut\bin"

del "..\WalnutApp\WalnutApp.vcxproj"
del "..\WalnutApp\WalnutApp.vcxproj.user"
del "..\WalnutApp\WalnutApp.vcxproj.filters"
rd /s /q "..\WalnutApp\bin"

PAUSE