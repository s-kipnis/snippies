
# place at the end of tests/unit/cmk/cee/dcd/test_connectors.py
def test_file_connector(config: Config) -> None:
    """To test conditionally available fileconnector
    place fileconnector side by side with example_connector and run test
    Such complicated metjod is used bec
    """
    from cmk.utils.hostaddress import HostName

    from .conftest import HostPartialConfig, TestSiteClient

    config.dcd_connections.from_config(
        {
            "fc_1": {
                "disabled": False,
                "connector": (
                    "fileconnector",
                    {
                        "site": "a",
                        "disabled": False,
                        "interval": 1,
                        "path": "",
                        "file_format": "",
                        "folder": "",
                        "lowercase_everything": False,
                        "replace_special_chars": False,
                        "host_filters": [],
                        "host_overtake_filters": [],
                        "chunk_size": 4,
                        "use_service_discovery": False,
                        "label_path_template": "",
                        "csv_delimiter": None,
                        "label_prefix": None,
                    },
                ),
                "site": "a",
            }
        }
    )
    logger = logging.getLogger()
    client = TestSiteClient(
        {
            HostName("pig1"): HostPartialConfig(
                folder="/home",
                attributes={
                    "locked_attributes": ["site", "tag_address_family", "tag_agent", "tag_snmp"],
                    "locked_by": {
                        "site_id": "my_site",
                        "program_id": "dcd",
                        "instance_id": "piggy",
                    },
                    "site": "my_site",
                    "tag_address_family": "no-ip",
                    "tag_agent": "no-agent",
                    "tag_snmp": "no-snmp",
                },
            ),
        }
    )

    try:
        from cmk.cee.dcd.plugins.connectors.fileconnector import ( # type: ignore[attr-defined]
            FileConnectorX,
        )

        _fc = FileConnector(TestEnvironment(logger, config), client, "fc_1", "a", FakeStore())  # type: ignore[name-defined]
    except ImportError:
        return
