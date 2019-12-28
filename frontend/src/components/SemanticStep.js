import React from 'react'


import { Icon, Step } from 'semantic-ui-react'


const step_enum = window.Module.step_enum;
const step_api = window.Module.step_api;


class SemanticStep extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      activeItem: step_api.data
    };
    this.handleStepChange = this.handleStepChange.bind(this)
    step_api.addListener(this.handleStepChange);
  }

  handleStepChange() {
    this.setState({activeItem: step_api.data});
  }


  render() {
    const { activeItem } = this.state;

    return (
      <div>
        <Step.Group size='small'>
          <Step id='step_input' active={activeItem === step_enum.input} disabled={activeItem < step_enum.input}>
            <Icon name='file alternate outline' />
            <Step.Content>
              <Step.Title>File Input</Step.Title>
            </Step.Content>
          </Step>

          <Step id='step_velveth' active={activeItem === step_enum.velveth} disabled={activeItem < step_enum.velveth}>
            <Icon name='edit' />
            <Step.Content>
              <Step.Title>velveth</Step.Title>
            </Step.Content>
          </Step>

          <Step id='step_velvetg' active={activeItem === step_enum.velvetg} disabled={activeItem < step_enum.velvetg}>
            <Icon name='exchange' />
            <Step.Content>
              <Step.Title>velvetg</Step.Title>
            </Step.Content>
          </Step>

          <Step id='step_complete' active={activeItem === step_enum.complete} disabled={activeItem < step_enum.complete}>
            {activeItem === step_enum.complete ? (
              <Icon name='check' color='green' />
            ) : (
              <Icon name='check' />
            )}
            <Step.Content>
              <Step.Title>Complete</Step.Title>
            </Step.Content>
          </Step>
        </Step.Group>
      </div>
    );
  }
}

export default SemanticStep;
