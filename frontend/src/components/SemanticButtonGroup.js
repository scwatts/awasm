import React from 'react'


import { Button, Loader, Icon } from 'semantic-ui-react'


const execute_assembly = window.execute_assembly;
const download_assembly = window.download_assembly;
const step_enum = window.Module.step_enum;
const step_api = window.Module.step_api;
const execution_api = window.Module.execution_api;


class SemanticButtonGroup extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      execution: execution_api.data,
      complete: false
    };

    this.handleExecutionChange = this.handleExecutionChange.bind(this)
    this.handleStepChange = this.handleStepChange.bind(this)

    execution_api.addListener(this.handleExecutionChange);
    step_api.addListener(this.handleStepChange);
  }

  handleExecutionChange() {
    this.setState({execution: execution_api.data});
  }

  handleStepChange() {
    this.setState({complete: step_api.data === step_enum.complete});
  }

  render() {
    const { execution, complete } = this.state;

    return(
      <>
        <Button id='button_assembly' icon disabled={!!execution} labelPosition='right' onClick={() => execute_assembly()}>
          Assemble!
          {execution ? (
            <Icon>
              <Loader active size='small' />
            </Icon>
          ) : (
            <Icon name='cog' />
          )}
        </Button>
        <Button id='button_download' icon color='blue' labelPosition='right' disabled={!complete} onClick={() => download_assembly()}>
          Download
          <Icon name='download' />
        </Button>
      </>
    );
  }
}


export default SemanticButtonGroup;
