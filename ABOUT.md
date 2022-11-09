# Почему .xml

Проще брать данные из этого
*xml*
```xml
<?xml version="1.0" encoding="UTF-8"?>
<document>
    <data id="marketdata">
        <rows>
            <row SECID="GAZP" OPEN="210" LOW="209.06" HIGH="216.88" LAST="213" HIGH="216.88" TIME="19:38:53" SYSTIME="2022-10-06 19:53:54" ISSUECAPITALIZATION_UPDATETIME="19:53:00" />
        </rows>
    </data>
</document>
```

чем, из json/csv или html

*json*
```json
{
    "marketdata": {
        "columns": [
            "SECID",
            "OPEN",
            "LOW",
            "HIGH",
            "LAST",
            "HIGH",
            "TIME",
            "SYSTIME",
            "ISSUECAPITALIZATION_UPDATETIME"
        ],
        "data": [
            [
                "GAZP",
                210,
                209.06,
                216.88,
                212.95,
                216.88,
                "19:48:20",
                "2022-10-06 20:03:24",
                "20:02:59"
            ]
        ]
    }
}
```
*csv*
```csv
marketdata

GAZP;210;209.06;216.88;212.9;216.88;19:48:39;2022-10-06 20:03:40;20:02:59
```
*html*
```html
<h1>marketdata</h1>
<table>
	<tr>
		<td>GAZP</td>
		<td>210</td>
		<td>209.06</td>
		<td>216.88</td>
		<td>212.96</td>
		<td>216.88</td>
		<td>19:50:27</td>
		<td>2022-10-06 20:05:28</td>
		<td>20:04:59</td>
	</tr>
</table>
<br /><br />
</table>
```
